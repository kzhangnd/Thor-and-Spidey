/* single.c: Single User HTTP Server */

#include "spidey.h"

#include <errno.h>
#include <string.h>

#include <unistd.h>

/**
 * Handle one HTTP request at a time.
 *
 * @param   sfd         Server socket file descriptor.
 * @return  Exit status of server (EXIT_SUCCESS).
 **/
int single_server(int sfd) {
    Request *r;
    Status result;
    
    /* Accept and handle HTTP request */
    while (true) {
        /* Accept request */
        debug("Accepting Request");
        r = accept_request(sfd);
        if (!r) {
            debug("Unable to accept request");
            continue;
        }

        /* Handle request */
        debug("Handling Request");
        result = handle_request(r);
        if (result != HTTP_STATUS_OK)
            log("Unable to handle request");

        /* Free request */
        debug("Freeing Request");
        free_request(r);
    }

    /* Close server socket */
    close(sfd);
    return EXIT_SUCCESS;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
