/* forking.c: Forking HTTP Server */

#include "spidey.h"

#include <errno.h>
#include <signal.h>
#include <string.h>

#include <unistd.h>

/**
 * Fork incoming HTTP requests to handle the concurrently.
 *
 * @param   sfd         Server socket file descriptor.
 * @return  Exit status of server (EXIT_SUCCESS).
 *
 * The parent should accept a request and then fork off and let the child
 * handle the request.
 **/
int forking_server(int sfd) {
    Request *r;
    Status result;
    /* Accept and handle HTTP request */
    while (true) {
    	/* Accept request */
        debug("Accepting Request");
        r = accept_request(sfd);

        /* Ignore children */
        pid_t pid = fork();
        if (pid < 0) {
            debug("fork failed: %s", strerror(errno));
            free_request(r);
            continue;
        }

        /* Fork off child process to handle request */
        if (pid == 0) {                // Child
            close(sfd);

            /* Handle request */
            debug("Handling Request");
            result = handle_request(r);
            if (result != HTTP_STATUS_OK)
                log("Unable to handle request");

            /* Free request */
            debug("Freeing Request");
            free_request(r);
            exit(EXIT_SUCCESS);
        }
        else {                          // Parent
            free_request(r);
        }

    }

    /* Close server socket */
    close(sfd);
    return EXIT_SUCCESS;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
