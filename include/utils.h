#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <microhttpd.h>

/* Maximum buffer size for POST requests */
#define POSTBUFFERSIZE 8192

/* Used HTTP response codes */
typedef enum {
	OK = 200,
	BAD_REQUEST = 400,
	NOT_FOUND = 404,
	INTERNAL_SERVER_ERROR = 500,
	NOT_IMPLEMENTED = 501
} HTTP_status;

/* Handles HTTP response body */
typedef struct {
	char *body;
	HTTP_status status;
} HTTP_response;

/* Handle incoming connection information */
typedef struct {
	struct MHD_PostProcessor *pp;
	char *json_data;
	size_t json_size;
} ConnectionInfo;

char* SimpleMessage(const char *message_str);
enum MHD_Result CreateResponse(struct MHD_Connection *con, const char *msg, unsigned int status, ConnectionInfo *con_info);
HTTP_response ValidateResult(char *result);

#endif
