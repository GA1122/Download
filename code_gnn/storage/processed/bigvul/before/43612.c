AP_DECLARE(request_rec *) ap_sub_req_method_uri(const char *method,
                                                const char *new_uri,
                                                const request_rec *r,
                                                ap_filter_t *next_filter)
{
    request_rec *rnew;
     
    int res = HTTP_INTERNAL_SERVER_ERROR;
    char *udir;

    rnew = make_sub_request(r, next_filter);

     
    rnew->method = method;
    rnew->method_number = ap_method_number_of(method);

    if (new_uri[0] == '/') {
        ap_parse_uri(rnew, new_uri);
    }
    else {
        udir = ap_make_dirstr_parent(rnew->pool, r->uri);
        udir = ap_escape_uri(rnew->pool, udir);     
        ap_parse_uri(rnew, ap_make_full_path(rnew->pool, udir, new_uri));
    }

     
    if (ap_is_recursion_limit_exceeded(r)) {
        rnew->status = HTTP_INTERNAL_SERVER_ERROR;
        return rnew;
    }

     
    if (next_filter) {
        res = ap_run_quick_handler(rnew, 1);
    }

    if (next_filter == NULL || res != OK) {
        if ((res = ap_process_request_internal(rnew))) {
            rnew->status = res;
        }
    }

    return rnew;
}