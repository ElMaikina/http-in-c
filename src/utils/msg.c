#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../../include/msg.h"

/*
Common functions to ensure responses comply with the HTTP standard.
There are also functions to facilitate code readability, such as 
functions to read API methods or routes.
*/

/* Return a simple message as an HTTP response */
char *SimpleMessage(const char *message_str) {
	char *formatted_message = NULL;
	size_t formatted_message_size = strlen(message_str) + 20;
	formatted_message = (char *)malloc(formatted_message_size);
	if (formatted_message)
		snprintf(formatted_message, formatted_message_size, "{\"message\": \"%s\"}", message_str);
	return formatted_message;
}

/* Auxiliary function to create an HTTP response */
enum MHD_Result CreateResponse(struct MHD_Connection *con, const char *msg, unsigned int status, ConnectionInfo *con_info) {
	struct MHD_Response *res = MHD_create_response_from_buffer(strlen(msg), (void *)msg, MHD_RESPMEM_PERSISTENT);
	MHD_add_response_header(res, "Content-Type", "application/json");
	MHD_add_response_header(res, "Access-Control-Allow-Origin", "*");
	MHD_add_response_header(res, "Access-Control-Allow-Methods", "*");
	MHD_add_response_header(res, "Access-Control-Allow-Headers", "*");
	enum MHD_Result ret = MHD_queue_response(con, status, res);
	MHD_destroy_response(res);
	free(con_info->json_data);
	free(con_info);
	return ret;
}

/* Validates result and return an HTTP response */
HTTP_response ValidateResult(char *result) {
	if (result == NULL) {
		return (HTTP_response){
			.body = SimpleMessage("Internal server error"),
			.status = INTERNAL_SERVER_ERROR
		};
	}
	return (HTTP_response){
		.body = result,
		.status = OK
	};
}
