static int core_pre_connection(conn_rec *c, void *csd)
{
    core_net_rec *net;
    apr_status_t rv;

    if (c->master) {
        return DONE;
    }
    
    net = apr_palloc(c->pool, sizeof(*net));
     
    rv = apr_socket_opt_set(csd, APR_TCP_NODELAY, 1);
    if (rv != APR_SUCCESS && rv != APR_ENOTIMPL) {
         
        ap_log_cerror(APLOG_MARK, APLOG_DEBUG, rv, c, APLOGNO(00139)
                      "apr_socket_opt_set(APR_TCP_NODELAY)");
    }

     
    rv = apr_socket_timeout_set(csd, c->base_server->timeout);
    if (rv != APR_SUCCESS) {
         
        ap_log_cerror(APLOG_MARK, APLOG_DEBUG, rv, c, APLOGNO(00140)
                      "apr_socket_timeout_set");
    }

    net->c = c;
    net->in_ctx = NULL;
    net->out_ctx = NULL;
    net->client_socket = csd;

    ap_set_core_module_config(net->c->conn_config, csd);
     
    if (c->master == NULL) {
        ap_add_input_filter_handle(ap_core_input_filter_handle, net, NULL,
                                   net->c);
        ap_add_output_filter_handle(ap_core_output_filter_handle, net, NULL,
                                    net->c);
    }
    return DONE;
}
