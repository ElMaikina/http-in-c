#ifndef TOKEN_H
#define TOKEN_H

#define SECRET_KEY "clave_super_secreta"

char* URLtoBase64(const unsigned char *input, int length);
char* CreateJWT(const char *correo);
int VerifyJWT(const char *token);
char* ObtainJWT(struct MHD_Connection *connection);

#endif
