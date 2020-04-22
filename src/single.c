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

      /* Handle request */
      debug("Handling Request");
      result = handle_request();

      /* Free request */
      debug("Freeing Request");
      free_request(r);
    }

    /* Close server socket */
    if (close(sfd) < 0){
      fprintf(stderr, "close socket failed: %s\n", strerror(status));
    }

    return EXIT_SUCCESS;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
