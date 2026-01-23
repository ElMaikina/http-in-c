#include "../include/utils.h"
#include <microhttpd.h>
#include <setjmp.h>
#include "../include/route.h"
#include "../include/user.h"

jmp_buf ExceptionBuffer;

/* Outputs the request and method called by the client */
void LogAPI(const char *url, const char *method) {
	printf("[%s] %s\n", method, url);
}

/*
The "MainController" function is used to redirect
calls based on the type of object being invoked.
(This is similar to routers in FastAPI.)
*/
enum MHD_Result MainController (
	void *cls, struct MHD_Connection *conn,
	const char *url, const char *method,
	const char *ver, const char *data,
	size_t *data_size, void **con_cls
)
{
	// If a POST request is called, a memory
	// buffer is created for incomming data.
	ConnectionInfo *con_info = *con_cls;

	// First time recieving data
	if (!con_info) {
		con_info = calloc(1, sizeof(ConnectionInfo));
		con_info->json_data = malloc(POSTBUFFERSIZE);
		con_info->json_data[0] = '\0';
		con_info->json_size = 0;
		*con_cls = (void *)con_info;
		return MHD_YES;
	}
	// Process the stream of incomming data
	if (*data_size > 0) {
		if (con_info->json_size + *data_size < POSTBUFFERSIZE - 1) {
			memcpy(con_info->json_data + con_info->json_size, data, *data_size);
			con_info->json_size += *data_size;
			con_info->json_data[con_info->json_size] = '\0';
		}
		*data_size = 0;
		return MHD_YES;
	}

	// Resolve call with CORS preflight
	if (strcmp(method, "OPTIONS") == 0)
		return CreateResponse(conn, "", MHD_HTTP_OK, con_info);

	// Log API calls from clients
	LogAPI(url, method);

	// Redirect to the specific controller for each object.
	// The response is generated after calling a valid API.
	if (setjmp(ExceptionBuffer) == 0)
	{
		if (strcmp(url, "/") == 0) {
			char *msg = SimpleMessage("Hello!");
			return CreateResponse(conn, msg, OK, con_info);
		}
		if (strstr(url, "/users") != NULL) {
			return UserController(url, method, conn, con_info);
		}
		char *msg = SimpleMessage("Not found");
		return CreateResponse(conn, msg, NOT_FOUND, con_info);
	}
	// Request was invalid or couldn't be processed
	char *msg = SimpleMessage("Error");
	unsigned int err = INTERNAL_SERVER_ERROR;
	return CreateResponse(conn, msg, err, con_info);
}
