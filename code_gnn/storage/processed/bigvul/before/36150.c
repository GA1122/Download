int mk_handler_read(int socket, struct client_session *cs)
{
    int bytes;
    int max_read;
    int available = 0;
    int new_size;
    int total_bytes = 0;
    char *tmp = 0;

    MK_TRACE("MAX REQUEST SIZE: %i", config->max_request_size);

 try_pending:

    available = cs->body_size - cs->body_length;
    if (available <= 0) {
         
        new_size = cs->body_size + config->transport_buffer_size;
        if (new_size > config->max_request_size) {
            MK_TRACE("Requested size is > config->max_request_size");
            mk_request_premature_close(MK_CLIENT_REQUEST_ENTITY_TOO_LARGE, cs);
            return -1;
        }

         
        if (cs->body == cs->body_fixed) {
            cs->body = mk_mem_malloc(new_size + 1);
            cs->body_size = new_size;
            memcpy(cs->body, cs->body_fixed, cs->body_length);
            MK_TRACE("[FD %i] New size: %i, length: %i",
                     cs->socket, new_size, cs->body_length);
        }
        else {
            MK_TRACE("[FD %i] Realloc from %i to %i",
                     cs->socket, cs->body_size, new_size);
            tmp = mk_mem_realloc(cs->body, new_size + 1);
            if (tmp) {
                cs->body = tmp;
                cs->body_size = new_size;
            }
            else {
                mk_request_premature_close(MK_SERVER_INTERNAL_ERROR, cs);
                return -1;
            }
        }
    }

     
    max_read = (cs->body_size - cs->body_length);
    bytes = mk_socket_read(socket, cs->body + cs->body_length, max_read);

    MK_TRACE("[FD %i] read %i", socket, bytes);

    if (bytes < 0) {
        if (errno == EAGAIN) {
            return 1;
        }
        else {
            mk_session_remove(socket);
            return -1;
        }
    }
    if (bytes == 0) {
        mk_session_remove(socket);
        return -1;
    }

    if (bytes > 0) {
        if (bytes > max_read) {
            MK_TRACE("[FD %i] Buffer still have data: %i",
                     cs->socket, bytes - max_read);

            cs->body_length += max_read;
            cs->body[cs->body_length] = '\0';
            total_bytes += max_read;

            goto try_pending;
        }
        else {
            cs->body_length += bytes;
            cs->body[cs->body_length] = '\0';

            total_bytes += bytes;
        }

        MK_TRACE("[FD %i] Retry total bytes: %i",
                 cs->socket, total_bytes);
        return total_bytes;
    }

    return bytes;
}