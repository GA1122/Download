static request_rec *make_sub_request(const request_rec *r,
                                     ap_filter_t *next_filter)
{
    apr_pool_t *rrp;
    request_rec *rnew;

    apr_pool_create(&rrp, r->pool);
    apr_pool_tag(rrp, "subrequest");
    rnew = apr_pcalloc(rrp, sizeof(request_rec));
    rnew->pool = rrp;

    rnew->hostname       = r->hostname;
    rnew->request_time   = r->request_time;
    rnew->connection     = r->connection;
    rnew->server         = r->server;
    rnew->log            = r->log;

    rnew->request_config = ap_create_request_config(rnew->pool);

     
    rnew->per_dir_config = r->server->lookup_defaults;

    rnew->htaccess = r->htaccess;
    rnew->allowed_methods = ap_make_method_list(rnew->pool, 2);

     
    ap_copy_method_list(rnew->allowed_methods, r->allowed_methods);

     
    if (next_filter) {
         
        rnew->input_filters = r->input_filters;
        rnew->proto_input_filters = r->proto_input_filters;
        rnew->output_filters = next_filter;
        rnew->proto_output_filters = r->proto_output_filters;
        ap_add_output_filter_handle(ap_subreq_core_filter_handle,
                                    NULL, rnew, rnew->connection);
    }
    else {
         
        rnew->proto_input_filters = r->proto_input_filters;
        rnew->proto_output_filters = r->proto_output_filters;

        rnew->input_filters = r->proto_input_filters;
        rnew->output_filters = r->proto_output_filters;
    }

    rnew->useragent_addr = r->useragent_addr;
    rnew->useragent_ip = r->useragent_ip;

     

    ap_set_sub_req_protocol(rnew, r);

     
    ap_run_create_request(rnew);

     
    rnew->used_path_info = AP_REQ_DEFAULT_PATH_INFO;

     
    rnew->kept_body = r->kept_body;

    return rnew;
}
