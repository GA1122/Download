static int default_handler(request_rec *r)
{
    conn_rec *c = r->connection;
    apr_bucket_brigade *bb;
    apr_bucket *e;
    core_dir_config *d;
    int errstatus;
    apr_file_t *fd = NULL;
    apr_status_t status;
     
    int bld_content_md5;

    d = (core_dir_config *)ap_get_core_module_config(r->per_dir_config);
    bld_content_md5 = (d->content_md5 == AP_CONTENT_MD5_ON)
                      && r->output_filters->frec->ftype != AP_FTYPE_RESOURCE;

    ap_allow_standard_methods(r, MERGE_ALLOW, M_GET, M_OPTIONS, M_POST, -1);

     
    if ((errstatus = ap_discard_request_body(r)) != OK) {
        return errstatus;
    }

    if (r->method_number == M_GET || r->method_number == M_POST) {
        if (r->finfo.filetype == APR_NOFILE) {
            ap_log_rerror(APLOG_MARK, APLOG_INFO, 0, r, APLOGNO(00128)
                          "File does not exist: %s",
                          apr_pstrcat(r->pool, r->filename, r->path_info, NULL));
            return HTTP_NOT_FOUND;
        }

         
        if (r->finfo.filetype == APR_DIR) {
            ap_log_rerror(APLOG_MARK, APLOG_INFO, 0, r, APLOGNO(00129)
                          "Attempt to serve directory: %s", r->filename);
            return HTTP_NOT_FOUND;
        }

        if ((r->used_path_info != AP_REQ_ACCEPT_PATH_INFO) &&
            r->path_info && *r->path_info)
        {
             
            ap_log_rerror(APLOG_MARK, APLOG_INFO, 0, r, APLOGNO(00130)
                          "File does not exist: %s",
                          apr_pstrcat(r->pool, r->filename, r->path_info, NULL));
            return HTTP_NOT_FOUND;
        }

         
        if (r->method_number != M_GET) {
            core_request_config *req_cfg;

            req_cfg = ap_get_core_module_config(r->request_config);
            if (!req_cfg->deliver_script) {
                 
                ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r, APLOGNO(00131)
                              "This resource does not accept the %s method.",
                              r->method);
                return HTTP_METHOD_NOT_ALLOWED;
            }
        }


        if ((status = apr_file_open(&fd, r->filename, APR_READ | APR_BINARY
#if APR_HAS_SENDFILE
                            | AP_SENDFILE_ENABLED(d->enable_sendfile)
#endif
                                    , 0, r->pool)) != APR_SUCCESS) {
            ap_log_rerror(APLOG_MARK, APLOG_ERR, status, r, APLOGNO(00132)
                          "file permissions deny server access: %s", r->filename);
            return HTTP_FORBIDDEN;
        }

        ap_update_mtime(r, r->finfo.mtime);
        ap_set_last_modified(r);
        ap_set_etag(r);
        ap_set_accept_ranges(r);
        ap_set_content_length(r, r->finfo.size);
        if (bld_content_md5) {
            apr_table_setn(r->headers_out, "Content-MD5",
                           ap_md5digest(r->pool, fd));
        }

        bb = apr_brigade_create(r->pool, c->bucket_alloc);

        if ((errstatus = ap_meets_conditions(r)) != OK) {
            apr_file_close(fd);
            r->status = errstatus;
        }
        else {
            e = apr_brigade_insert_file(bb, fd, 0, r->finfo.size, r->pool);

#if APR_HAS_MMAP
            if (d->enable_mmap == ENABLE_MMAP_OFF) {
                (void)apr_bucket_file_enable_mmap(e, 0);
            }
#endif
        }

        e = apr_bucket_eos_create(c->bucket_alloc);
        APR_BRIGADE_INSERT_TAIL(bb, e);

        status = ap_pass_brigade(r->output_filters, bb);
        apr_brigade_cleanup(bb);

        if (status == APR_SUCCESS
            || r->status != HTTP_OK
            || c->aborted) {
            return OK;
        }
        else {
             
            ap_log_rerror(APLOG_MARK, APLOG_DEBUG, status, r, APLOGNO(00133)
                          "default_handler: ap_pass_brigade returned %i",
                          status);
            return AP_FILTER_ERROR;
        }
    }
    else {               
        if (r->method_number == M_INVALID) {
             
            if (r->the_request
                && r->the_request[0] == 0x16
                && (r->the_request[1] == 0x2 || r->the_request[1] == 0x3)) {
                ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r, APLOGNO(00134)
                              "Invalid method in request %s - possible attempt to establish SSL connection on non-SSL port", r->the_request);
            } else {
                ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r, APLOGNO(00135)
                              "Invalid method in request %s", r->the_request);
            }
            return HTTP_NOT_IMPLEMENTED;
        }

        if (r->method_number == M_OPTIONS) {
            return ap_send_http_options(r);
        }
        return HTTP_METHOD_NOT_ALLOWED;
    }
}