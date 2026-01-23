#include <stdio.h>
#include <string.h>
#include <microhttpd.h>
#include <unistd.h>
#include <curl/curl.h>
#include "../include/route.h"

#define PORT 8000

/*
Main function that opens a small server using libmicrohttp.
The server runs from an external process, so each call uses
its own thread in parallel.

(For more information, see the "MainManager" function in route.c)
*/
int main() {
	printf("Starting Web Server...\n");
	struct MHD_Daemon *daemon;
	daemon = MHD_start_daemon(MHD_USE_THREAD_PER_CONNECTION, PORT,
		NULL, NULL, &MainController, NULL, MHD_OPTION_END);
	printf("Open: http://localhost:%d/\n", PORT);
	while (1) {
		sleep(1);
	}
	MHD_stop_daemon(daemon);
	return 0;
}
