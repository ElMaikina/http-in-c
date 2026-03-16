#ifndef USER_H
#define USER_H

#include "msg.h"

typedef struct {
	long long id;
    char *name;
    char *email;
    char *password;
} User;

User* CreateUser(const char* json_str);
void FreeUser(User *u);
void InsertUser(User *u);
void UpdateUser(User *u, long long id);
void DeleteUser(long long id);
char* ViewAllUsers();
char* ViewLastUser();
char* SelectUser(long long id);
char* ValidateCredentials(char *mail, char *password);
enum MHD_Result UserController(const char *url, const char *method, struct MHD_Connection *conn, ConnectionInfo *con_info);

#endif
