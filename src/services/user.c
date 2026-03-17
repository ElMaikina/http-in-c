#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "../../include/user.h"

bool EmailExists(char *mail) {
	const char* result = SelectUserByEmail(mail);
    return (result != NULL);
}

long long GetIdFromLogin(char *mail, char *pass) {
	long long id = SelectIdByCredentials(mail, pass);
    return id;
}