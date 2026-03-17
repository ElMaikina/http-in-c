#ifndef QUERY_H
#define QUERY_H

#include <stdbool.h>
#include <mysql/mysql.h>

// Environment variables to connect to MySQL
#define MYSQL_HOST "localhost"
#define MYSQL_USER "root"

// Database name for this project
#define DB "BackEnd_in_C"

bool QueryToVoid(const char *query);
char* QueryToJSONArray(const char *query);
char* QueryToJSONObject(const char *query);
long long QueryToId(const char *query);
char* ResultToJSONArray(MYSQL_RES *result);
char* ResultToJSONObject(MYSQL_RES *result);

#endif
