#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "../../include/query.h"
#include "../../include/cache.h"
#include "../../include/crypto.h"
#include "../../include/user.h"

/* View all users in database */
char* SelectAllUsers() {
	char *cache = SearchInCache("users");
	if (!cache) {
		const char *query = "SELECT * FROM users";
		char *result = QueryToJSONArray(query);
		StoreInCache("users", result);
		return result;
	}
	return cache;
}

/* Muestra la ultima User insertada */
char* SelectLastUser() {	
	const char *query = "SELECT * FROM users ORDER BY id DESC LIMIT 1";
	char *result = QueryToJSONObject(query);
	return result;
}

/* Muestra una User con cierto ID */
char* SelectUserById(long long id) {
	char query[256];
	snprintf(query, sizeof(query), "SELECT * FROM users WHERE id = %lld", id);
	char *result = QueryToJSONObject(query);
	return result;
}

/* Insert user into database  */
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

/* Update user from database */
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

/* Delete user from database */
void DeleteUser(long long id) {
    char query[256];
	snprintf(query, sizeof(query), "\
	DELETE FROM users\
	WHERE id = %lld", id);
	QueryToVoid(query);
	return;
}