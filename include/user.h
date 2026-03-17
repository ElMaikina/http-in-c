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
char* SelectUserByEmail(char *mail);
long long SelectIdByCredentials(char *mail, char *pass);

/* Services and business logic */
// bool CredentialsAreValid(char *mail, char *password);
// bool PasswordIsValid(char *password);
// bool EmailIsInvalid(char *mail);
bool EmailExists(char *mail);
long long GetIdFromLogin(char *mail, char *pass);

/* Controller / router */
enum MHD_Result UserController(const char *url, const char *method, struct MHD_Connection *conn, ConnectionInfo *con_info);

#endif