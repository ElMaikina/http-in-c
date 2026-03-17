#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <microhttpd.h>
#include <openssl/evp.h>
#include "../../include/token.h"
#include "../../include/auth.h"
#include "../../include/user.h"
#include "../../include/msg.h"

/* Manages calls related to the users */
enum MHD_Result AuthController (
	const char *url, const char *method,
	struct MHD_Connection *conn, ConnectionInfo *con_info,
	EVP_PKEY *key
)
{
	if (strcmp(method, "POST") == 0) {
		if (strcmp(url, "/auth/signup") == 0) {
			User *u = CreateUser(con_info->json_data);
			if (!u) {
				char *msg = SimpleMessage("Invalid body format!");
				return CreateResponse(conn, msg, BAD_REQUEST, con_info);
			}
			if (EmailExists(u->email)) {
				char *msg = SimpleMessage("User with email already exists!");
				return CreateResponse(conn, msg, BAD_REQUEST, con_info);
			}
			InsertUser(u);
			FreeUser(u);
			char *msg = SimpleMessage("User created succesfully!");
			return CreateResponse(conn, msg, MHD_HTTP_OK, con_info);
		}
		if (strcmp(url, "/auth/login") == 0) {
			User *u = CreateUser(con_info->json_data);
			if (!u) {
				char *msg = SimpleMessage("Invalid body format!");
				return CreateResponse(conn, msg, BAD_REQUEST, con_info);
			}
			long long id = GetIdFromLogin(u->email, u->password);
			if (!id) {
				char *msg = SimpleMessage("Wrong or invalid credentials!");
				return CreateResponse(conn, msg, BAD_REQUEST, con_info);
			}
			u->id = id;
			const char *jwt = CreateJWT(id, key);
			char result[512];
			snprintf(result, sizeof(result), "{\"token\": \"%s\"}", jwt);
			return CreateResponse(conn, result, MHD_HTTP_OK, con_info);
		}
	}
	char *msg = SimpleMessage("Invalid request!");
	unsigned int err = MHD_HTTP_BAD_REQUEST;
	return CreateResponse(conn, msg, err, con_info);
}
