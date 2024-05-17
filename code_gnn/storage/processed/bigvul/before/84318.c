static void close_and_free_request(struct wsgi_request *wsgi_req) {

        if (!wsgi_req->fd_closed) {
                wsgi_req->socket->proto_close(wsgi_req);
        }

        if (wsgi_req->post_file) {
                fclose(wsgi_req->post_file);
        }

        if (wsgi_req->post_read_buf) {
                free(wsgi_req->post_read_buf);
        }

        if (wsgi_req->post_readline_buf) {
                free(wsgi_req->post_readline_buf);
        }

        if (wsgi_req->proto_parser_buf) {
                free(wsgi_req->proto_parser_buf);
        }

}
