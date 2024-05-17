lrmd_init_remote_tls_server(int port)
{
    int rc;
    int filter;
    struct addrinfo hints, *res = NULL, *iter;
    char port_str[16];

    static struct mainloop_fd_callbacks remote_listen_fd_callbacks = {
        .dispatch = lrmd_remote_listen,
        .destroy = lrmd_remote_connection_destroy,
    };

    crm_notice("Starting a tls listener on port %d.", port);
    crm_gnutls_global_init();
    gnutls_global_set_log_function(debug_log);

    gnutls_dh_params_init(&dh_params);
    gnutls_dh_params_generate2(dh_params, 1024);
    gnutls_psk_allocate_server_credentials(&psk_cred_s);
    gnutls_psk_set_server_credentials_function(psk_cred_s, lrmd_tls_server_key_cb);
    gnutls_psk_set_server_dh_params(psk_cred_s, dh_params);

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_flags = AI_PASSIVE;  
    hints.ai_family = AF_UNSPEC;  
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    snprintf(port_str, sizeof(port_str), "%d", port);
    rc = getaddrinfo(NULL, port_str, &hints, &res);
    if (rc) {
        crm_err("getaddrinfo: %s", gai_strerror(rc));
        return -1;
    }

    iter = res;
    filter = AF_INET6;
     
    while (iter) {
        if (iter->ai_family == filter) {
            ssock = bind_and_listen(iter);
        }
        if (ssock != -1) {
            break;
        }

        iter = iter->ai_next;
        if (iter == NULL && filter == AF_INET6) {
            iter = res;
            filter = AF_INET;
        }
    }

    if (ssock < 0) {
        crm_err("unable to bind to address");
        goto init_remote_cleanup;
    }

    mainloop_add_fd("lrmd-remote", G_PRIORITY_DEFAULT, ssock, NULL, &remote_listen_fd_callbacks);

    rc = ssock;
  init_remote_cleanup:
    if (rc < 0) {
        close(ssock);
        ssock = 0;
    }
    freeaddrinfo(res);
    return rc;

}