static int mk_request_header_process(struct session_request *sr)
{
    int uri_init = 0, uri_end = 0;
    int query_init = 0;
    int prot_init = 0, prot_end = 0, pos_sep = 0;
    int fh_limit;
    char *headers;
    char *temp = 0;
    mk_ptr_t host;

     
    sr->method_p = mk_http_method_check_str(sr->method);

     
    temp = index(sr->body.data, ' ');
    if (mk_unlikely(!temp)) {
        MK_TRACE("Error, invalid first header");
        return -1;
    }
    uri_init = (temp - sr->body.data) + 1;

    temp = index(sr->body.data, '\n');
    if (mk_unlikely(!temp)) {
        MK_TRACE("Error, invalid header CRLF");
        return -1;
    }
    fh_limit = (temp - sr->body.data);

    uri_end = mk_string_char_search_r(sr->body.data, ' ', fh_limit) - 1;
    if (mk_unlikely(uri_end <= 0)) {
        MK_TRACE("Error, first header bad formed");
        return -1;
    }

    prot_init = uri_end + 2;

    if (mk_unlikely(uri_end < uri_init)) {
        return -1;
    }

     
    query_init = mk_string_char_search(sr->body.data + uri_init, '?', prot_init - uri_init);
    if (query_init > 0) {
        int init, end;

        init = query_init + uri_init;
        if (init <= uri_end) {
            end = uri_end;
            uri_end = init - 1;

            sr->query_string = mk_ptr_create(sr->body.data,
                                                 init + 1, end + 1);
        }
    }

     
    sr->uri = mk_ptr_create(sr->body.data, uri_init, uri_end + 1);
    if (mk_unlikely(sr->uri.len < 1)) {
        return -1;
    }

     
    prot_end = fh_limit - 1;
    if (mk_unlikely(prot_init == prot_end)) {
        return  -1;
    }

    if (prot_end != prot_init && prot_end > 0) {
        sr->protocol = mk_http_protocol_check(sr->body.data + prot_init,
                                              prot_end - prot_init);
        sr->protocol_p = mk_http_protocol_check_str(sr->protocol);
    }

    headers = sr->body.data + prot_end + mk_crlf.len;

     
    temp = mk_utils_url_decode(sr->uri);
    if (temp) {
        sr->uri_processed.data = temp;
        sr->uri_processed.len  = strlen(temp);
    }
    else {
        sr->uri_processed.data = sr->uri.data;
        sr->uri_processed.len  = sr->uri.len;
    }

     
    sr->headers_len = sr->body.len - (prot_end + mk_crlf.len);
    if (mk_request_header_toc_parse(&sr->headers_toc, headers, sr->headers_len) < 0) {
        MK_TRACE("Invalid headers");
        return -1;
    }

     
    host = mk_request_header_get(&sr->headers_toc,
                                 mk_rh_host.data,
                                 mk_rh_host.len);
    if (host.data) {
        if ((pos_sep = mk_string_char_search_r(host.data, ':', host.len)) >= 0) {
             
            char *p;
            short int port_len, port_size = 6;
            char port[port_size];

             
            sr->host.data = host.data;
            sr->host.len = pos_sep;

             
            sr->host_port = host;

             
            port_len = (host.len - pos_sep - 1);
            if (port_len >= port_size) {
                return -1;
            }

             
            memcpy(port, host.data + pos_sep + 1, port_len);
            port[port_len] = '\0';

             
            p = port;
            while (*p) {
                if (!isdigit(*p)) return -1;
                p++;
            }

             
            errno = 0;
            sr->port = strtol(port, (char **) NULL, 10);
            if ((errno == ERANGE && (sr->port == LONG_MAX || sr->port == LONG_MIN))
                || sr->port == 0) {
                return -1;
            }
        }
        else {
            sr->host = host;     
            sr->port = config->standard_port;
        }
    }
    else {
        sr->host.data = NULL;
    }

     
    sr->connection = mk_request_header_get(&sr->headers_toc,
                                           mk_rh_connection.data,
                                           mk_rh_connection.len);

    sr->range = mk_request_header_get(&sr->headers_toc,
                                      mk_rh_range.data,
                                      mk_rh_range.len);

    sr->if_modified_since = mk_request_header_get(&sr->headers_toc,
                                                  mk_rh_if_modified_since.data,
                                                  mk_rh_if_modified_since.len);

     
    if (sr->protocol == MK_HTTP_PROTOCOL_10) {
        sr->keep_alive = MK_FALSE;
        sr->close_now = MK_TRUE;
    }
    else if(sr->protocol == MK_HTTP_PROTOCOL_11) {
        sr->keep_alive = MK_TRUE;
        sr->close_now = MK_FALSE;
    }
    if (sr->connection.data) {
        if (mk_string_search_n(sr->connection.data, "Keep-Alive",
                               MK_STR_INSENSITIVE, sr->connection.len) >= 0) {
            sr->keep_alive = MK_TRUE;
            sr->close_now = MK_FALSE;
        }
        else if (mk_string_search_n(sr->connection.data, "Close",
                                    MK_STR_INSENSITIVE, sr->connection.len) >= 0) {
            sr->keep_alive = MK_FALSE;
            sr->close_now = MK_TRUE;
        }
        else {
             
            sr->connection.len = 0;
        }
    }

    return 0;
}
