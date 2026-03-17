#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <mysql/mysql.h>
#include "../../include/query.h"

/*
MySQL / MariaDB is used to store the data on the hard drive.
As data is added, it is cached to avoid overloading the hard drive
(for more info. view functions declared in cache.c)
*/

/* Escape all double quotes from strings */
char *EscapeQuotes(const char *input) {
	if (!input) 
		return NULL;
	size_t len = strlen(input);
	size_t extra = 0;
	for (size_t i = 0; i < len; i++) {
		if (input[i] == '"')
			extra++;
	}
	char *result = malloc(len + extra + 1);
	size_t j = 0;
	for (size_t i = 0; i < len; i++) {
		if (input[i] == '"') {
			result[j++] = '\\';
			result[j++] = '"';
		} 
		else {
			result[j++] = input[i];
		}
	}
	result[j] = '\0';
	return result;
}


/* Run query in MySQL and discard the result */
bool QueryToVoid(const char *query) {
	MYSQL *con = mysql_init(NULL);
	if (con == NULL) {
		return false;
	}
	if (mysql_real_connect(con, MYSQL_HOST, MYSQL_USER, NULL, DB, 0, NULL, 0) == NULL) {
		return false;
	}
	if (mysql_query(con, query)) {
		return false;
	}
	mysql_close(con);
	return true;
}

/* Run query in MySQL and return a result in a JSON array */
char *QueryToJSONArray(const char *query) {
	MYSQL *con = mysql_init(NULL);
	if (con == NULL) {
		return NULL;
	}
	if (mysql_real_connect(con, MYSQL_HOST, MYSQL_USER, NULL, DB, 0, NULL, 0) == NULL) {
		return NULL;
	}
	if (mysql_query(con, query)) {
		return NULL;
	}
	MYSQL_RES *result = mysql_store_result(con);
	if (result == NULL) {
		return NULL;
	}
	char *json = ResultToJSONArray(result);
	mysql_free_result(result);
	mysql_close(con);
	return json;
}

/* Run query in MySQL and return a result in a JSON object */
char *QueryToJSONObject(const char *query) {
	MYSQL *con = mysql_init(NULL);
	if (con == NULL) {
		return NULL;
	}
	if (mysql_real_connect(con, MYSQL_HOST, MYSQL_USER, NULL, DB, 0, NULL, 0) == NULL) {
		return NULL;
	}
	if (mysql_query(con, query)) {
		return NULL;
	}
	MYSQL_RES *result = mysql_store_result(con);
	if (result == NULL) {
		return NULL;
	}
	char *json = ResultToJSONObject(result);
	mysql_free_result(result);
	mysql_close(con);
	return json;
}

/* Extract only the row id from a query */
long long QueryToId(const char *query) {
	long long id = 0;
	MYSQL *con = mysql_init(NULL);
	if (con == NULL) {
		return id;
	}
	if (mysql_real_connect(con, MYSQL_HOST, MYSQL_USER, NULL, DB, 0, NULL, 0) == NULL) {
		return id;
	}
	if (mysql_query(con, query)) {
		return id;
	}
	MYSQL_RES *result = mysql_store_result(con);
	if (result == NULL) {
		return id;
	}
	int num_fields = mysql_num_fields(result);
	int num_rows = mysql_num_rows(result);
	if (num_rows != 1) {
		return id;
	}
	if (num_fields >= 1) {
		MYSQL_ROW row = mysql_fetch_row(result);
		const MYSQL_FIELD *field = mysql_fetch_field_direct(result, 0);
		if (IS_NUM(field->type) && row[0])
			id = (long long)row[0];
	}
	mysql_free_result(result);
	mysql_close(con);
	return id;
}

/* Take the MySQL result and convert it to a JSON array */
char *ResultToJSONArray(MYSQL_RES *result) {
	int num_fields = mysql_num_fields(result);
	int num_rows = mysql_num_rows(result);
	int curr_row = 0;
	int total_size = num_rows * (2 + num_fields * 256) + num_rows - 1 + 3;
	char *json = (char *)malloc(total_size);
	json[0] = '\0';
	if (num_rows == 0) {
		strcat(json, "[]");
		return json;
	}
	MYSQL_ROW row;
	strcat(json, "[");
	while ((row = mysql_fetch_row(result))) {
		strcat(json, "{");
		for (int i = 0; i < num_fields; i++) {
			MYSQL_FIELD *field = mysql_fetch_field_direct(result, i);
			strcat(json, "\"");
			strncat(json, field->name, total_size - strlen(json) - 5);
			strcat(json, "\":");
			if (IS_NUM(field->type))
				strncat(json, row[i] ? row[i] : "\"NULL\"", total_size - strlen(json) - 5);
			else {
				strcat(json, "\"");
				char *my_row = EscapeQuotes(row[i]);
				strncat(json, my_row ? my_row : "NULL", total_size - strlen(json) - 5);
				strcat(json, "\"");
			}
			if (i < num_fields - 1)
				strcat(json, ",");
		}
		strcat(json, "}");
		if (curr_row < num_rows - 1)
			strcat(json, ",");
		curr_row++;
	}
	strcat(json, "]");
	return json;
}

/* Take the MySQL result and convert it to a JSON object */
char *ResultToJSONObject(MYSQL_RES *result) {
	int num_fields = mysql_num_fields(result);
	int num_rows = mysql_num_rows(result);
	int total_size = num_rows * (2 + num_fields * 256) + num_rows - 1 + 3;
	char *json = (char *)malloc(total_size);
	json[0] = '\0';
	if (num_rows == 0) {
		return NULL;
	}
	MYSQL_ROW row = mysql_fetch_row(result);
	strcat(json, "{");
	for (int i = 0; i < num_fields; i++) {
		MYSQL_FIELD *field = mysql_fetch_field_direct(result, i);
		strcat(json, "\"");
		strncat(json, field->name, total_size - strlen(json) - 5);
		strcat(json, "\":");
		if (IS_NUM(field->type))
			strncat(json, row[i] ? row[i] : "\"NULL\"", total_size - strlen(json) - 5);
		else {
			strcat(json, "\"");
			char *my_row = EscapeQuotes(row[i]);
			strncat(json, my_row ? my_row : "NULL", total_size - strlen(json) - 5);
			strcat(json, "\"");
		}
		if (i < num_fields - 1)
			strcat(json, ",");
	}
	strcat(json, "}");
	return json;
}
