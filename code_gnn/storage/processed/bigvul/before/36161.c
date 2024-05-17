static int mk_request_process(struct client_session *cs, struct session_request *sr)
{
    int status = 0;
    int socket = cs->socket;
    struct mk_list *hosts = &config->hosts;
    struct mk_list *alias;

     
    sr->host_conf = mk_list_entry_first(hosts, struct host, _head);

     
    status = mk_request_header_process(sr);
    if (status < 0) {
        mk_header_set_http_status(sr, MK_CLIENT_BAD_REQUEST);
        mk_request_error(MK_CLIENT_BAD_REQUEST, cs, sr);
        return EXIT_ABORT;
    }

    sr->user_home = MK_FALSE;

     
    if (sr->uri_processed.data[0] != '/') {
        mk_request_error(MK_CLIENT_BAD_REQUEST, cs, sr);
        return EXIT_NORMAL;
    }

     
    if (!sr->host.data && sr->protocol == MK_HTTP_PROTOCOL_11) {
        mk_request_error(MK_CLIENT_BAD_REQUEST, cs, sr);
        return EXIT_NORMAL;
    }

     
    if (sr->protocol == MK_HTTP_PROTOCOL_UNKNOWN) {
        mk_request_error(MK_SERVER_HTTP_VERSION_UNSUP, cs, sr);
        return EXIT_ABORT;
    }

     
    alias = &sr->host_conf->server_names;
    sr->host_alias = mk_list_entry_first(alias,
                                         struct host_alias, _head);

    if (sr->host.data) {
         
        mk_vhost_get(sr->host, &sr->host_conf, &sr->host_alias);

         
        if (sr->host_conf->header_redirect.data) {
            mk_header_set_http_status(sr, MK_REDIR_MOVED);
            sr->headers.location = mk_string_dup(sr->host_conf->header_redirect.data);
            sr->headers.content_length = 0;
            mk_header_send(cs->socket, cs, sr);
            sr->headers.location = NULL;
            mk_server_cork_flag(cs->socket, TCP_CORK_OFF);
            return 0;
        }
    }

     
    if (config->user_dir &&
        sr->uri_processed.len > 2 &&
        sr->uri_processed.data[1] == MK_USER_HOME) {

        if (mk_user_init(cs, sr) != 0) {
            mk_request_error(MK_CLIENT_NOT_FOUND, cs, sr);
            return EXIT_ABORT;
        }
    }

     
    if (sr->method == MK_HTTP_METHOD_POST || sr->method == MK_HTTP_METHOD_PUT) {
        if ((status = mk_method_parse_data(cs, sr)) != 0) {
            return status;
        }
    }

     
    int ret;
    ret = mk_plugin_stage_run(MK_PLUGIN_STAGE_20, socket, NULL, cs, sr);
    if (ret == MK_PLUGIN_RET_CLOSE_CONX) {
        MK_TRACE("STAGE 20 requested close conexion");
        return EXIT_ABORT;
    }

     
    status = mk_http_init(cs, sr);

    MK_TRACE("[FD %i] HTTP Init returning %i", socket, status);

    return status;
}