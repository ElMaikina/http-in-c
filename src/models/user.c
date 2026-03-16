#include <cjson/cJSON.h>
#include "../../include/user.h"
#include "../../include/query.h"

/*
Creates an user struct from a JSON input.
Returns NULL if it fails.
*/
User *CreateUser(const char* json_str) {
    cJSON *json = cJSON_Parse(json_str);
    if (!json) {
        return NULL;
    }
    User *u = malloc(sizeof(User));
    if (cJSON* name = cJSON_GetObjectItemCaseSensitive(json, "name")) {
        u->name = strdup(name->valuestring);
    }
    else {
        return NULL;
    }
    if (cJSON* email = cJSON_GetObjectItemCaseSensitive(json, "email")) {
        u->email = strdup(email->valuestring);
    }
    else {
        return NULL;
    }
    if (cJSON* password = cJSON_GetObjectItemCaseSensitive(json, "password")) {
        u->password = strdup(password->valuestring);
    }
    else {
        return NULL;
    }
    cJSON_Delete(json);
    return u;
}

/* Free memory from a user struct */
void FreeUser(User *u) {
    free(u->name);
    free(u->email);
    free(u->password);
}