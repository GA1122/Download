AP_DECLARE(request_rec *) ap_sub_req_lookup_dirent(const apr_finfo_t *dirent,
                                                   const request_rec *r,
                                                   int subtype,
                                                   ap_filter_t *next_filter)
{
    request_rec *rnew;
    int res;
    char *fdir;
    char *udir;

    rnew = make_sub_request(r, next_filter);

     
    if (r->path_info && *r->path_info) {
         
        udir = apr_pstrdup(rnew->pool, r->uri);
        udir[ap_find_path_info(udir, r->path_info)] = '\0';
        udir = ap_make_dirstr_parent(rnew->pool, udir);

        rnew->uri = ap_make_full_path(rnew->pool, udir, dirent->name);
        if (subtype == AP_SUBREQ_MERGE_ARGS) {
            rnew->uri = ap_make_full_path(rnew->pool, rnew->uri, r->path_info + 1);
            rnew->path_info = apr_pstrdup(rnew->pool, r->path_info);
        }
        rnew->uri = ap_escape_uri(rnew->pool, rnew->uri);
    }
    else {
        udir = ap_make_dirstr_parent(rnew->pool, r->uri);
        rnew->uri = ap_escape_uri(rnew->pool, ap_make_full_path(rnew->pool,
                                                                udir,
                                                                dirent->name));
    }

    fdir = ap_make_dirstr_parent(rnew->pool, r->filename);
    rnew->filename = ap_make_full_path(rnew->pool, fdir, dirent->name);
    if (r->canonical_filename == r->filename) {
        rnew->canonical_filename = rnew->filename;
    }

     
    rnew->per_dir_config = r->server->lookup_defaults;

    if ((dirent->valid & APR_FINFO_MIN) != APR_FINFO_MIN) {
         
        apr_status_t rv;
        if (ap_allow_options(rnew) & OPT_SYM_LINKS) {
            if (((rv = apr_stat(&rnew->finfo, rnew->filename,
                                APR_FINFO_MIN, rnew->pool)) != APR_SUCCESS)
                && (rv != APR_INCOMPLETE)) {
                rnew->finfo.filetype = APR_NOFILE;
            }
        }
        else {
            if (((rv = apr_stat(&rnew->finfo, rnew->filename,
                                APR_FINFO_LINK | APR_FINFO_MIN,
                                rnew->pool)) != APR_SUCCESS)
                && (rv != APR_INCOMPLETE)) {
                rnew->finfo.filetype = APR_NOFILE;
            }
        }
    }
    else {
        memcpy(&rnew->finfo, dirent, sizeof(apr_finfo_t));
    }

    if (rnew->finfo.filetype == APR_LNK) {
         
        if ((res = resolve_symlink(rnew->filename, &rnew->finfo,
                                   ap_allow_options(rnew), rnew->pool))
            != OK) {
            rnew->status = res;
            return rnew;
        }
    }

    if (rnew->finfo.filetype == APR_DIR) {
         
        strcat(rnew->filename, "/");
        if (!rnew->path_info || !*rnew->path_info) {
            strcat(rnew->uri, "/");
        }
    }

     
    if (r->args && *r->args && (subtype == AP_SUBREQ_MERGE_ARGS)) {
        ap_parse_uri(rnew, apr_pstrcat(r->pool, rnew->uri, "?",
                                       r->args, NULL));
    }
    else {
        ap_parse_uri(rnew, rnew->uri);
    }

     
    if (ap_is_recursion_limit_exceeded(r)) {
        rnew->status = HTTP_INTERNAL_SERVER_ERROR;
        return rnew;
    }

    if ((res = ap_process_request_internal(rnew))) {
        rnew->status = res;
    }

    return rnew;
}
