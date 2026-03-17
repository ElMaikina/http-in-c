#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/ec.h>
#include <openssl/bn.h>
#include <openssl/sha.h>
#include <openssl/buffer.h>
#include <openssl/bio.h>
#include <microhttpd.h>

#define PAYLOAD_SIZE 256

char *Base64URL(const unsigned char *input, int length) {
    BIO *b64, *bmem;
    BUF_MEM *bptr;

    b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);

    bmem = BIO_new(BIO_s_mem());
    b64 = BIO_push(b64, bmem);

    BIO_write(b64, input, length);
    BIO_flush(b64);

    BIO_get_mem_ptr(b64, &bptr);

    char *buff = malloc(bptr->length + 1);
    memcpy(buff, bptr->data, bptr->length);
    buff[bptr->length] = 0;

    BIO_free_all(b64);

    for (char *p = buff; *p; p++) {
        if (*p == '+')
            *p = '-';
        else if (*p == '/')
            *p = '_';
    }

    while (buff[strlen(buff)-1] == '=')
        buff[strlen(buff)-1] = 0;

    return buff;
}

unsigned char *Base64URLDecode(const char *input, int *out_len) {
    int len = strlen(input);
    int pad = (4 - len % 4) % 4;
    char *tmp = malloc(len + pad + 1);
    strcpy(tmp,input);

    for (int i=0; i < pad; i++)
        strcat(tmp,"=");
    
    for (char *p=tmp; *p; p++) {
        if (*p=='-')
            *p='+';
        else if(*p=='_')
            *p='/';
    }

    BIO *b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    BIO *mem = BIO_new_mem_buf(tmp, -1);
    mem = BIO_push(b64, mem);
    unsigned char *buffer = malloc(len);
    *out_len = BIO_read(mem, buffer, len);
    BIO_free_all(mem);
    free(tmp);
    return buffer;
}

EVP_PKEY *GenerateES256() {
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_EC, NULL);
    EVP_PKEY *pkey = NULL;
    EVP_PKEY_keygen_init(ctx);
    EVP_PKEY_CTX_set_ec_paramgen_curve_nid(ctx, NID_X9_62_prime256v1);
    EVP_PKEY_keygen(ctx, &pkey);
    EVP_PKEY_CTX_free(ctx);
    return pkey;
}

unsigned char *Sign (
    EVP_PKEY *key, const unsigned char *data,
    size_t data_len, size_t *sig_len
)
{
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    EVP_DigestSignInit(ctx, NULL, EVP_sha256(), NULL, key);
    EVP_DigestSign(ctx, NULL, sig_len, data, data_len);
    unsigned char *sig = malloc(*sig_len);
    EVP_DigestSign(ctx,sig, sig_len, data, data_len);
    EVP_MD_CTX_free(ctx);
    return sig;
}

int Verify (
    EVP_PKEY *key, const unsigned char *data,
    size_t data_len, unsigned char *sig, size_t sig_len
)
{
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    EVP_DigestVerifyInit(ctx, NULL,EVP_sha256(), NULL, key);
    int ok = EVP_DigestVerify(ctx, sig, sig_len, data, data_len);
    EVP_MD_CTX_free(ctx);
    return ok == 1;
}

void DER_a_RS(unsigned char *der,size_t der_len,unsigned char out[64]) {
    const unsigned char *p = der;
    ECDSA_SIG *sig = d2i_ECDSA_SIG(NULL,&p,der_len);
    const BIGNUM *r,*s;
    ECDSA_SIG_get0(sig,&r,&s);
    BN_bn2binpad(r, out, 32);
    BN_bn2binpad(s, out+32, 32);
    ECDSA_SIG_free(sig);
}

unsigned char *RS_a_DER(unsigned char rs[64],size_t *der_len){
    BIGNUM *r = BN_bin2bn(rs, 32, NULL);
    BIGNUM *s = BN_bin2bn(rs+32, 32, NULL);
    ECDSA_SIG *sig = ECDSA_SIG_new();
    ECDSA_SIG_set0(sig,r,s);
    *der_len = i2d_ECDSA_SIG(sig, NULL);
    unsigned char *der = malloc(*der_len);
    unsigned char *p = der;
    i2d_ECDSA_SIG(sig, &p);
    ECDSA_SIG_free(sig);
    return der;
}

char *CreateJWT(long long user_id, EVP_PKEY *key) {
    const char *header = "{\"alg\":\"ES256\",\"typ\":\"JWT\"}";
    char payload[PAYLOAD_SIZE];
    snprintf(payload, sizeof(payload), "%lld", user_id);
    char *h64 = Base64URL((unsigned char*)header, strlen(header));
    char *p64 = Base64URL((unsigned char*)payload, strlen(payload));
    char signing_input[512];
    sprintf(signing_input, "%s.%s", h64, p64);
    size_t der_len;
    unsigned char *der = Sign (
        key, (unsigned char*)signing_input,
        strlen(signing_input), &der_len
    );
    unsigned char rs[64];
    DER_a_RS(der, der_len, rs);
    char *sig64 = Base64URL(rs, 64);
    char *jwt = malloc(strlen(signing_input)+strlen(sig64)+2);
    sprintf(jwt, "%s.%s", signing_input, sig64);
    free(h64);
    free(p64);
    free(sig64);
    free(der);
    return jwt;
}

int VerifyJWT(const char *token, EVP_PKEY *key) {
    char *copy = strdup(token);
    char *header = strtok(copy,".");
    char *payload = strtok(NULL,".");
    char *sig64 = strtok(NULL,".");

    if(!header || !payload || !sig64) {
        free(copy);
        return 0;
    }

    char signing_input[512];
    sprintf(signing_input, "%s.%s", header, payload);

    int sig_len;
    unsigned char *sig = Base64URLDecode(sig64, &sig_len);

    if(sig_len != 64) {
        free(sig);
        free(copy);
        return 0;
    }

    size_t der_len;
    unsigned char *der = RS_a_DER(sig, &der_len);

    int ok = Verify (
        key, (unsigned char*)signing_input,
        strlen(signing_input), der, der_len
    );

    free(sig);
    free(der);
    free(copy);
    return ok;
}

long long GetUserIdFromJWT(struct MHD_Connection *conn) {
    long long user_id = 0;
    int len;
	const char *auth_header = MHD_lookup_connection_value
		(conn, MHD_HEADER_KIND, "Authorization");
	if (auth_header) {
        char id_str[256];
        char *copy = strdup(auth_header);
        char *bearer = strtok(copy, " ");
        char *jwt = strtok(NULL, " ");
        char *header = strtok(jwt, ".");
        char *payload = strtok(NULL, ".");
        unsigned char *json = Base64URLDecode(payload, &len);
        snprintf(id_str, sizeof(id_str), "%.*s\n", len, json);
        user_id = strtoll(id_str, NULL, 0);
        free(copy);
    }
    return user_id;
}
