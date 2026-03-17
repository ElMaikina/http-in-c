#ifndef TASK_H
#define TASK_H

#include "msg.h"

typedef struct {
	long long id;
    char *details;
	long long user_id;
} Task;

Task* CreateTask(const char* json_str);
void FreeTask(Task *t);
void InsertTask(Task *t);
void UpdateTask(Task *t, long long id);
void DeleteTask(long long id);
char* SelectAllTasksByUser();
char* SelectTaskById(long long id);
enum MHD_Result TaskController(const char *url, const char *method, struct MHD_Connection *conn, ConnectionInfo *con_info);

#endif
