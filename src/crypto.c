#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/crypto.h"

// Encrypt password (using XOR method)
char *EncryptPassword(const char *password) {
    size_t len = strlen(password);
    char *enc = malloc(len + 1);
    for (size_t i = 0; i < len; i++)
        enc[i] = password[i] ^ XOR_KEY;
    enc[len] = '\0';
    return enc;
}

// Decrypt password (using XOR method)
char *DecryptPassword(const char *encrypted) {
    return EncryptPassword(encrypted); // XOR reversible
}
