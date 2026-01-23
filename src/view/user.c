#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "../../include/query.h"
#include "../../include/cache.h"
#include "../../include/crypto.h"
#include "../../include/user.h"

/* View all users in database */
char* ViewAllUsers() {
	char *cache = SearchInCache("users");
	if (!cache) {
		const char *query = "SELECT * FROM users";
		char *result = QueryToJSON(query);
		StoreInCache("users", result);
		return result;
	}
	return cache;
}

/* Muestra la ultima User insertada */
char* ViewLastUser() {	
	const char *query = "SELECT * FROM users ORDER BY ID DESC LIMIT 1";
	char *result = QueryToJSON(query);
	return result;
}

/* Muestra una User con cierto ID */
char* SelectUser(long long id) {
	char query[256];
	snprintf(query, sizeof(query), "SELECT * FROM users WHERE ID = %lld", id);
	char *result = QueryToJSON(query);
	return result;
}
