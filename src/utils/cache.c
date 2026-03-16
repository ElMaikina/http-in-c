#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <hiredis/hiredis.h>
#include "../../include/cache.h"

/*
Redis is used as an in-memory cache dictionary to quickly
access values that have already been computed and formatted
in the views.
*/

/* Attempts to connect to the in-memory cache dictionary */
redisContext *CacheConnection() {
	redisContext *conn = redisConnect(CACHE_HOST, CACHE_PORT);
	if (conn->err) {
		printf("error: %s\n", conn->errstr);
		return NULL;
	}
	return conn;
}

/* Looks up a value in the memory cache dictionary */
char* SearchInCache(const char *key) {
	char *res = NULL;
	redisContext *conn = CacheConnection();
	if (conn) {
		redisReply *reply = redisCommand(conn, "GET %s", key);
		if (reply->str) {
			size_t len = strlen(reply->str) * 2; // Hack to account for UTF-8
			res = malloc(sizeof(char) * len);
			strcpy(res, reply->str);
		}
		freeReplyObject(reply);
		redisFree(conn);
	}
	return res;
}

/* Stores a value in the memory cache dictionary */
void StoreInCache(const char *key, const char *value) {
	redisContext *conn = CacheConnection();
	if (conn) {
		redisReply *reply = redisCommand(conn, "SET %s %s", key, value);
		freeReplyObject(reply);
		redisFree(conn);
	}
}
