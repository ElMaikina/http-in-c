#ifndef QUERY_H
#define QUERY_H

#include <mysql/mysql.h>

// Environment variables to connect to MySQL
#define MYSQL_HOST "localhost"
#define MYSQL_USER "root"

// Database name for this project
#define DB "BackEnd_in_C"

char* QueryToJSONArray(const char *query);
char* ResultToJSONArray(MYSQL_RES *result);

#endif
