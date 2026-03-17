#ifndef AUTH_H
#define AUTH_H

#include <openssl/evp.h>
#include <stdbool.h>
#include "msg.h"

enum MHD_Result AuthController (
	const char *url, const char *method,
	struct MHD_Connection *conn, ConnectionInfo *con_info,
	EVP_PKEY *key
);

#endif