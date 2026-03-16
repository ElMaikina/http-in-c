#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <microhttpd.h>
#include "../../include/user.h"
#include "../../include/msg.h"

/* Manages calls related to the users */
enum MHD_Result UserController (
	const char *url, const char *method,
	struct MHD_Connection *conn, ConnectionInfo *con_info
)
{
	long long id;
	sscanf(url, "/users/%lld", &id);
	
	if (strcmp(method, "GET") == 0) {
		if (id) {
			char* result = SelectUser(id);
			return CreateResponse(conn, result, MHD_HTTP_OK, con_info);
		}
		char* result = ViewAllUsers();
		return CreateResponse(conn, result, MHD_HTTP_OK, con_info);
	}
	if (strcmp(method, "POST") == 0) {
		User *u = CreateUser(con_info->json_data);
		if (!u) {
			char *msg = SimpleMessage("Couldn't create user!");
			return CreateResponse(conn, msg, BAD_REQUEST, con_info);
		}
		InsertUser(u);
		FreeUser(u);
		char* result = ViewLastUser();
		return CreateResponse(conn, result, MHD_HTTP_OK, con_info);
	}
	if (strcmp(method, "PUT") == 0 && id) {
		User *u = CreateUser(con_info->json_data);
		if (!u) {
			char *msg = SimpleMessage("Couldn't modify user!");
			return CreateResponse(conn, msg, BAD_REQUEST, con_info);
		}
		UpdateUser(u, id);
		FreeUser(u);
		char *result = SelectUser(id);
		return CreateResponse(conn, result, MHD_HTTP_OK, con_info);
	}
	if (strcmp(method, "DELETE") == 0 && id) {
		char *result = SelectUser(id);
		DeleteUser(id);
		return CreateResponse(conn, result, MHD_HTTP_OK, con_info);
	}
	char *msg = SimpleMessage("Invalid request!");
	unsigned int err = MHD_HTTP_BAD_REQUEST;
	return CreateResponse(conn, msg, err, con_info);
}
