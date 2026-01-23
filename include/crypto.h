#ifndef CRYPTO_H
#define CRYPTO_H

#define XOR_KEY 0x5A // clave ligera

char* EncryptPassword(const char *password);
char* DecryptPassword(const char *encrypted);

#endif
