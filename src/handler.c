/* handler.c: HTTP Request Handlers */

#include "spidey.h"

#include <errno.h>
#include <limits.h>
#include <string.h>

#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

/* Internal Declarations */
Status handle_browse_request(Request *request);
Status handle_file_request(Request *request);
Status handle_cgi_request(Request *request);
Status handle_error(Request *request, Status status);

/**
 * Handle HTTP Request.
 *
 * @param   r           HTTP Request structure
 * @return  Status of the HTTP request.
 *
 * This parses a request, determines the request path, determines the request
 * type, and then dispatches to the appropriate handler type.
 *
 * On error, handle_error should be used with an appropriate HTTP status code.
 **/
Status  handle_request(Request *r) {
    Status result;

    /* Parse request */
    if (parse_request(r) < 0) {
        debug("Unable to parse request");
        return handle_error(r, HTTP_STATUS_BAD_REQUEST);
    }

    /* Determine request path */
    r->path = determine_request_path(r->uri);
    if (!r->path) {
        debug("Unable to determine full path");
        return handle_error(r, HTTP_STATUS_NOT_FOUND);
    }
    debug("HTTP REQUEST PATH: %s", r->path);

    /* Dispatch to appropriate request handler type based on file type */
    struct stat s;
    if (stat(r->path, &s) < 0) {
        debug("File path doesn't exist: %s", strerror(errno));
        return handle_error(r, HTTP_STATUS_NOT_FOUND);
    }

    if (S_ISREG(s.st_mode) == 0)
        result = handle_browse_request(r);
    else if (access(r->path, X_OK) == 0)
        result = handle_cgi_request(r);
    else if (access(r->path, R_OK) == 0)
        result = handle_file_request(r);

    log("HTTP REQUEST STATUS: %s", http_status_string(result));

    if (result != HTTP_STATUS_OK)
        return handle_error(r, result);
    else
        return result;
}

/**
 * Handle browse request.
 *
 * @param   r           HTTP Request structure.
 * @return  Status of the HTTP browse request.
 *
 * This lists the contents of a directory in HTML.
 *
 * If the path cannot be opened or scanned as a directory, then handle error
 * with HTTP_STATUS_NOT_FOUND.
 **/
Status  handle_browse_request(Request *r) {
    log("Handling browse request");
    struct dirent **entries;
    int n;

    /* Open a directory for reading or scanning */
    n = scandir(r->path, &entries, NULL, alphasort);
    if (n < 0) {
        debug("scandir: %s\n", strerror(errno));
        return HTTP_STATUS_NOT_FOUND;
    }

    /* Write HTTP Header with OK Status and text/html Content-Type */
    fprintf(r->stream, "HTTP/1.0 200 OK\n");
    fprintf(r->stream, "Content-Type: text/html\n");
    fprintf(r->stream, "\r\n");


    fprintf(r->stream, "<html>\n");

    /* Write head part*/
    fprintf(r->stream, "<head>\n");
    fprintf(r->stream, "<meta charset=\"utf-8\">\n");
    fprintf(r->stream, "<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n");
    fprintf(r->stream, "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1, shrink-to-fit=no\">\n");
    fprintf(r->stream, "<link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.4.1/css/bootstrap.min.css\" integrity=\"sha384-Vkoo8x4CGsO3+Hhxv8T/Q5PaXtkKtu6ug5TOeNV6gBiFeWPGFN9MuhOf23Q9Ifjh\" crossorigin=\"anonymous\">");
    fprintf(r->stream, "</head>\n");

    /* For each entry in directory, emit HTML list item */
    fprintf(r->stream, "<body>\n");
    fprintf(r->stream, "<div class=\"container\">\n");
    fprintf(r->stream, "<div class=\"row\">\n");
    fprintf(r->stream, "<div class=\"col-sm-4\">\n");
    fprintf(r->stream, "<div class=\"list-group\">\n");

    char buffer[BUFSIZ];
    for (int i = 0; i < n; i++) {
        if (streq(entries[i]->d_name, ".")) {
            free(entries[i]);
            continue;
        }

        if (r->uri[strlen(r->uri) - 1] == '/')
            sprintf(buffer, "%s%s", r->uri, entries[i]->d_name);
        else
            sprintf(buffer, "%s/%s", r->uri, entries[i]->d_name);

        if (streq(entries[i]->d_name, ".."))
            fprintf(r->stream, "<a href=\"%s\" class=\"list-group-item list-group-item-action list-group-item-info\">%s</a>\n", buffer, entries[i]->d_name);
        else {
            /* Check if the file is an image*/
            char *mimetype = determine_mimetype(entries[i]->d_name);
            if (strncmp(mimetype, "image", 5) == 0) {
                fprintf(r->stream, "<a href=\"%s\" class = \"list-group-item list-group-item-action list-group-item-primary\">%s\n", buffer, entries[i]->d_name);
                fprintf(r->stream, "<img src = \"%s\" class=\"img-thumbnail\" alt = \"%s\">\n", buffer, entries[i]->d_name);
                fprintf(r->stream, "</a>");
            }
            /* Display item name*/
            else
                fprintf(r->stream, "<a href=\"%s\" class=\"list-group-item list-group-item-action list-group-item-primary\">%s</a>\n", buffer, entries[i]->d_name);
            free(mimetype);
        }

     	free(entries[i]);
    }
    fprintf(r->stream, "</div>\n");
    fprintf(r->stream, "</body>\n");
    fprintf(r->stream, "</html>\n");
    free(entries);

    /* Return OK */
    return HTTP_STATUS_OK;
}

/**
 * Handle file request.
 *
 * @param   r           HTTP Request structure.
 * @return  Status of the HTTP file request.
 *
 * This opens and streams the contents of the specified file to the socket.
 *
 * If the path cannot be opened for reading, then handle error with
 * HTTP_STATUS_NOT_FOUND.
 **/
Status  handle_file_request(Request *r) {
    log("Handling file request");
    FILE *fs;
    char buffer[BUFSIZ];
    char *mimetype = NULL;
    size_t nread;

    /* Open file for reading */
    fs = fopen(r->path, "r");
    if (!fs) {
    	debug("fopen: %s", strerror(errno));
    	return HTTP_STATUS_NOT_FOUND;
    }

    /* Determine mimetype */
    mimetype = determine_mimetype(r->path);

    /* Write HTTP Headers with OK status and determined Content-Type */
    fprintf(r->stream, "HTTP/1.0 200 OK\n");
    fprintf(r->stream, "Content-Type: %s\n", mimetype);
    fprintf(r->stream, "\r\n");

    /* Read from file and write to socket in chunks */
    nread = fread(buffer, 1, BUFSIZ, fs);
    while (nread > 0) {
        if (fwrite(buffer, 1, nread, r->stream) != nread) {
            debug("Unable to open file");
            goto fail;
        }
        nread = fread(buffer, 1, BUFSIZ, fs);
    }

    /* Close file, deallocate mimetype, return OK */
    fclose(fs);
    free(mimetype);
    return HTTP_STATUS_OK;

fail:
    /* Close file, free mimetype, return INTERNAL_SERVER_ERROR */
    if (fs)
        fclose(fs);
    if (mimetype)
        free(mimetype);
    return HTTP_STATUS_INTERNAL_SERVER_ERROR;
}

/**
 * Handle CGI request
 *
 * @param   r           HTTP Request structure.
 * @return  Status of the HTTP file request.
 *
 * This popens and streams the results of the specified executables to the
 * socket.
 *
 * If the path cannot be popened, then handle error with
 * HTTP_STATUS_INTERNAL_SERVER_ERROR.
 **/
Status  handle_cgi_request(Request *r) {
    FILE *pfs;
    char buffer[BUFSIZ];

    /* Export CGI environment variables from request:
     * http://en.wikipedia.org/wiki/Common_Gateway_Interface */
    setenv("DOCUMENT_ROOT", RootPath, 1);
    setenv("QUERY_STRING", r->query, 1);
    setenv("REMOTE_ADDR", r->host, 1);
    setenv("REMOTE_PORT", r->port, 1);
    setenv("REQUEST_METHOD", r->method, 1);
    setenv("REQUEST_URI", r->uri, 1);
    setenv("SCRIPT_FILENAME", r->path, 1);
    setenv("SERVER_PORT", Port, 1);

    /* Export CGI environment variables from request headers */
    for (Header *header = r->headers; header; header = header->next) {
    	if (streq(header->name, "Host"))
            setenv("HTTP_HOST", header->data, 1);
        else if (streq(header->name, "Accept"))
            setenv("HTTP_ACCEPT", header->data, 1);
        else if (streq(header->name, "Accept-Language"))
            setenv("HTTP_ACCEPT_LANGUAGE", header->data, 1);
        else if (streq(header->name, "Accept-Encoding"))
            setenv("HTTP_ACCEPT_ENCODING", header->data, 1);
        else if (streq(header->name, "Connection"))
            setenv("HTTP_CONNECTION", header->data, 1);
        else if (streq(header->name, "User-Agent"))
            setenv("HTTP_USER_AGENT", header->data, 1);
    }

    /* POpen CGI Script */
    pfs = popen(r->path, "r");
    if (!pfs) {
        debug("Unable to popen CGI Scipt: %s", strerror(errno));
        return HTTP_STATUS_INTERNAL_SERVER_ERROR;
    }

    /* Copy data from popen to socket */
    while (fgets(buffer, BUFSIZ, pfs)) {
        fputs(buffer, r->stream);
    }

    /* Close popen, return OK */
    pclose(pfs);
    return HTTP_STATUS_OK;
}

/**
 * Handle displaying error page
 *
 * @param   r           HTTP Request structure.
 * @return  Status of the HTTP error request.
 *
 * This writes an HTTP status error code and then generates an HTML message to
 * notify the user of the error.
 **/
Status  handle_error(Request *r, Status status) {
    char *status_string = (char *)http_status_string(status);
    debug("Error type: %s", status_string);

    char *number = strdup(status_string);
    char *word = strchr(number, ' ');
    *(word++) = '\0';

    /* Write HTTP Header */
    fprintf(r->stream, "HTTP/1.0 %s\n", status_string);
    fprintf(r->stream, "Content-Type: text/html\n");
    fprintf(r->stream, "\r\n");

    /* Write head part*/
    fprintf(r->stream, "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1, shrink-to-fit=no\">\n");
    fprintf(r->stream, "<link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.4.1/css/bootstrap.min.css\" integrity=\"sha384-Vkoo8x4CGsO3+Hhxv8T/Q5PaXtkKtu6ug5TOeNV6gBiFeWPGFN9MuhOf23Q9Ifjh\" crossorigin=\"anonymous\">");

    /* Write HTML Description of Error*/
    fprintf(r->stream, "<div class=\"page-wrap d-flex flex-row align-items-center\">\n");
    fprintf(r->stream, "<div class=\"container\">\n");
    fprintf(r->stream, "<div class=\"row justify-content-center\">");
    fprintf(r->stream, "<div class=\"col-md-12 text-center\">");
    fprintf(r->stream, "<span class=\"display-1 d-block\">%s</span>\n", number);
    fprintf(r->stream, "<div class=\"mb-4 lead\">%s</div>\n", word);
    fprintf(r->stream, "<a href=\"/\" class=\"btn btn-link\">Back to Home</a>");
    fprintf(r->stream, "</div>\n");
    fprintf(r->stream, "</div>\n");
    fprintf(r->stream, "</div>\n");
    fprintf(r->stream, "</div>\n");

    free(number);

    /* Return specified status */
    return status;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
