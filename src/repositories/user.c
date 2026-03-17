#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "../../include/query.h"
#include "../../include/cache.h"
#include "../../include/crypto.h"
#include "../../include/user.h"

char* SelectAllUsers() {
	const char *query = "SELECT * FROM users";
	char *result = QueryToJSONArray(query);
	StoreInCache("users", result);
	return result;
}

char* SelectLastUser() {	
	const char *query = "SELECT * FROM users ORDER BY id DESC LIMIT 1";
	char *result = QueryToJSONObject(query);
	return result;
}

char* SelectUserById(long long id) {
	char query[256];
	snprintf(query, sizeof(query), "SELECT * FROM users WHERE id = %lld", id);
	char *result = QueryToJSONObject(query);
	return result;
}

void InsertUser(User *u) {
    char query[256];
    snprintf(query, sizeof(query), "\
        INSERT INTO users\
        (email, password)\
        VALUES ('%s', '%s')",
        u->email,
        u->password
    );
	QueryToVoid(query);
	return;
}

void UpdateUser(User *u, long long id) {
    char query[256];
    snprintf(query, sizeof(query), "\
        UPDATE users SET\
        email = '%s',\
        password = '%s'\
        WHERE id = %lld",
        u->email,
        u->password,
        id
    );
    QueryToVoid(query);
	return;
}

void DeleteUser(long long id) {
    char query[256];
	snprintf(query, sizeof(query), "\
	DELETE FROM users\
	WHERE id = %lld", id);
	QueryToVoid(query);
	return;
}

char* SelectUserByEmail(char *mail) {
	char query[256];
	snprintf(query, sizeof(query), "\
    SELECT * FROM users\
    WHERE email = '%s'\
    LIMIT 1", mail);
	char *result = QueryToJSONObject(query);
	return result;
}

long long SelectIdByCredentials(char *mail, char *pass) {
	char query[256];
	snprintf(query, sizeof(query), "\
    SELECT id FROM users\
    WHERE email = '%s'\
    AND password = '%s'\
    LIMIT 1", mail, pass);
	long long id = QueryToId(query);
	return id;
}