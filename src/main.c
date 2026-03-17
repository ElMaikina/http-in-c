#include <stdio.h>
#include <string.h>
#include <microhttpd.h>
#include <openssl/evp.h>
#include <setjmp.h>
#include <unistd.h>
#include <curl/curl.h>
#include "../include/msg.h"
#include "../include/user.h"
#include "../include/auth.h"
#include "../include/token.h"
#include <time.h>

#define PORT 8000

jmp_buf ExceptionBuffer;

/* Outputs the request and method called by the client */
void OutputLogs(const char *url, const char *method) {
	time_t mytime = time(NULL);
    char * time_str = ctime(&mytime);
    time_str[strlen(time_str)-1] = '\0';
	printf("%s: [%s] %s\n", time_str, method, url);
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
	// The EVP key is read from the controller
	// and is shared between all connections
	EVP_PKEY *key = (EVP_PKEY *)cls;

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
	OutputLogs(url, method);

	// Redirect to the specific controller for each object
	// The response is generated after calling a valid API
	if (setjmp(ExceptionBuffer) == 0)
	{
		if (strcmp(url, "/") == 0) {
			char *msg = SimpleMessage("Web Server is running!");
			return CreateResponse(conn, msg, OK, con_info);
		}
		if (strstr(url, "/auth") != NULL) {
			return AuthController(url, method, conn, con_info, key);
		}
		/*
		if (strstr(url, "/users") != NULL) {
			return UserController(url, method, conn, con_info);
		}
		*/
		char *msg = SimpleMessage("Resource not found");
		return CreateResponse(conn, msg, NOT_FOUND, con_info);
	}
	// Request was invalid or couldn't be processed
	char *msg = SimpleMessage("Error");
	unsigned int err = INTERNAL_SERVER_ERROR;
	return CreateResponse(conn, msg, err, con_info);
}

/*
Main function that opens a small server using libmicrohttp.
The server runs from an external process, so each call uses
its own thread in parallel.
*/
int main() {
	printf("Starting Web Server...\n");
	EVP_PKEY *key = GenerateES256();
	struct MHD_Daemon *daemon;
	daemon = MHD_start_daemon(MHD_USE_THREAD_PER_CONNECTION, PORT,
		NULL, NULL, &MainController, key, MHD_OPTION_END);
	printf("Open: http://localhost:%d/\n", PORT);
	while (1) {
		sleep(1);
	}
	MHD_stop_daemon(daemon);
    EVP_PKEY_free(key);
	return 0;
}
