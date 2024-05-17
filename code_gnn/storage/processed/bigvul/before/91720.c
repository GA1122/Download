int am_read_post_data(request_rec *r, char **data, apr_size_t *length)
{
    apr_size_t bytes_read;
    apr_size_t bytes_left;
    apr_size_t len;
    long read_length;
    int rc;

     
    rc = ap_setup_client_block(r, REQUEST_CHUNKED_DECHUNK);
    if (rc != OK) {
        return rc;
    }

     
    if (!ap_should_client_block(r)) {
        len = 0;
    } else {
        len = r->remaining;
    }

    if (len >= 1024*1024) {
        AM_LOG_RERROR(APLOG_MARK, APLOG_ERR, 0, r,
                      "Too large POST data payload (%lu bytes).",
                      (unsigned long)len);
        return HTTP_BAD_REQUEST;
    }


    if (length != NULL) {
        *length = len;
    }

    *data = (char *)apr_palloc(r->pool, len + 1);
    if (*data == NULL) {
        AM_LOG_RERROR(APLOG_MARK, APLOG_ERR, 0, r,
                      "Failed to allocate memory for %lu bytes of POST data.",
                      (unsigned long)len);
        return HTTP_INTERNAL_SERVER_ERROR;
    }

     
    (*data)[len] = '\0';

    bytes_read = 0;
    bytes_left = len;

    while (bytes_left > 0) {
         
        read_length = ap_get_client_block(r, &(*data)[bytes_read],
                                          bytes_left);
        if (read_length == 0) {
             
            (*data)[bytes_read] = '\0';

            if (length != NULL) {
                *length = bytes_read;
            }
            break;
        }
        else if (read_length < 0) {
            AM_LOG_RERROR(APLOG_MARK, APLOG_ERR, 0, r,
                          "Failed to read POST data from client.");
            return HTTP_INTERNAL_SERVER_ERROR;
        }

        bytes_read += read_length;
        bytes_left -= read_length;
    }

    am_diag_printf(r, "POST data: %s\n", *data);
    return OK;
}
