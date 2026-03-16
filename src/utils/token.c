#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/hmac.h>
#include <openssl/buffer.h>
#include <microhttpd.h>
#include "../../include/token.h"

/* Converts the URL to base64url */
char *URLtoBase64(const unsigned char *input, int length) {
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

    // URL-safe
    for (char *p = buff; *p; p++) {
        if (*p == '+')
            *p = '-';
        else if (*p == '/')
            *p = '_';
    }
    return buff;
}

/* Creates a simple JWT */
char *CreateJWT(const char *correo) {
    const char *header = "{\"alg\":\"HS256\",\"typ\":\"JWT\"}";
    char payload[256];
    snprintf(payload, sizeof(payload), "{\"correo\":\"%s\"}", correo);

    char *header64 = URLtoBase64((const unsigned char *)header, strlen(header));
    char *payload64 = URLtoBase64((const unsigned char *)payload, strlen(payload));

    char to_sign[512];
    snprintf(to_sign, sizeof(to_sign), "%s.%s", header64, payload64);

    unsigned char *signature = HMAC (
        EVP_sha256(),
        SECRET_KEY,
        strlen(SECRET_KEY),
        (unsigned char *)to_sign,
        strlen(to_sign),
        NULL,
        NULL
    );
    char *signature64 = URLtoBase64(signature, 32);

    char *token = malloc(strlen(to_sign) + strlen(signature64) + 2);
    sprintf(token, "%s.%s", to_sign, signature64);

    free(header64);
    free(payload64);
    free(signature64);

    return token;
}

/* Verifies if the JWT is valid */
int VerifyJWT(const char *token) {
    char *copy = strdup(token);
    char *header = strtok(copy, ".");
    char *payload = strtok(NULL, ".");
    char *signature = strtok(NULL, ".");

    if (!header || !payload || !signature) {
        free(copy);
        return 0;
    }

    char to_sign[512];
    snprintf(to_sign, sizeof(to_sign), "%s.%s", header, payload);

    unsigned char *expected_sig = HMAC (
        EVP_sha256(),
        SECRET_KEY,
        strlen(SECRET_KEY),
        (unsigned char *)to_sign,
        strlen(to_sign),
        NULL,
        NULL
    );
    char *expected_sig64 = URLtoBase64(expected_sig, 32);

    int valid = strcmp(signature, expected_sig64) == 0;

    free(copy);
    free(expected_sig64);
    return valid;
}

/* Obtains the JWT from the HTTP header */
char *ObtainJWT(struct MHD_Connection *connection) {
	const char *auth_header = MHD_lookup_connection_value(connection, MHD_HEADER_KIND, "Authorization");
	if (!auth_header) {
		return NULL;
	}
	const char *prefix = "Bearer ";
	size_t prefix_len = strlen(prefix);
	if (strncmp(auth_header, prefix, prefix_len) != 0) {
		return NULL;
	}
	const char *token_start = auth_header + prefix_len;
	char *token = strdup(token_start);
	return token;
}
