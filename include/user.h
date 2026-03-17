#ifndef USER_H
#define USER_H

#include <stdbool.h>
#include "msg.h"

/* Struct that represents the user class */
typedef struct {
	long long id;
    char *email;
    char *password;
} User;

/* Object creation and functions */
User* CreateUser(const char* json_str);
void FreeUser(User *u);

/* MySQL queries */
void InsertUser(User *u);
void UpdateUser(User *u, long long id);
void DeleteUser(long long id);
char* SelectAllUsers();
char* SelectLastUser();
char* SelectUserById(long long id);

/* Services and business logic */
bool CredentialsAreValid(char *mail, char *password);
bool PasswordIsValid(char *password);
bool EmailIsValid(char *mail);

/* Controller / router */
enum MHD_Result UserController(const char *url, const char *method, struct MHD_Connection *conn, ConnectionInfo *con_info);

#endif