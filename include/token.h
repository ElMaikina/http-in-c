#ifndef TOKEN_H
#define TOKEN_H

#include <openssl/evp.h>

EVP_PKEY *GenerateES256();
char* Base64URL(const unsigned char *input, int length);
char* CreateJWT(long long user_id, EVP_PKEY *key);
int VerifyJWT(const char *token, EVP_PKEY *key);
long long GetIdFromJWT(struct MHD_Connection *conn);

#endif
