AP_DECLARE(int) ap_directory_walk(request_rec *r)
{
    ap_conf_vector_t *now_merged = NULL;
    core_server_config *sconf =
        ap_get_core_module_config(r->server->module_config);
    ap_conf_vector_t **sec_ent = (ap_conf_vector_t **) sconf->sec_dir->elts;
    int num_sec = sconf->sec_dir->nelts;
    walk_cache_t *cache;
    char *entry_dir;
    apr_status_t rv;
    int cached;

     
    if (r->filename == NULL) {
        ap_log_rerror(APLOG_MARK, APLOG_INFO, 0, r, APLOGNO(00029)
                      "Module bug?  Request filename is missing for URI %s",
                      r->uri);
       return OK;
    }

     
    if ((rv = apr_filepath_merge(&entry_dir, NULL, r->filename,
                                 APR_FILEPATH_NOTRELATIVE, r->pool))
                  != APR_SUCCESS) {
        ap_log_rerror(APLOG_MARK, APLOG_INFO, 0, r, APLOGNO(00030)
                      "Module bug?  Request filename path %s is invalid or "
                      "or not absolute for uri %s",
                      r->filename, r->uri);
        return OK;
    }

     
    r->filename = entry_dir;

    cache = prep_walk_cache(AP_NOTE_DIRECTORY_WALK, r);
    cached = (cache->cached != NULL);

     
    if (r->finfo.filetype == APR_NOFILE || r->finfo.filetype == APR_LNK) {
        rv = ap_run_dirwalk_stat(&r->finfo, r, APR_FINFO_MIN);

         
        if ((rv != APR_SUCCESS) ||
            (r->finfo.filetype != APR_NOFILE &&
             (r->finfo.filetype != APR_DIR) &&
             (r->filename[strlen(r->filename) - 1] == '/'))) {
             r->finfo.filetype = APR_NOFILE;  
        }
    }

    if (r->finfo.filetype == APR_REG) {
        entry_dir = ap_make_dirstr_parent(r->pool, entry_dir);
    }
    else if (r->filename[strlen(r->filename) - 1] != '/') {
        entry_dir = apr_pstrcat(r->pool, r->filename, "/", NULL);
    }

     
    if (cached
        && ((r->finfo.filetype == APR_REG)
            || ((r->finfo.filetype == APR_DIR)
                && (!r->path_info || !*r->path_info)))
        && (cache->dir_conf_tested == sec_ent)
        && (strcmp(entry_dir, cache->cached) == 0)) {
        int familiar = 0;

         
        if (r->per_dir_config == cache->per_dir_result) {
            familiar = 1;
        }

        if (r->per_dir_config == cache->dir_conf_merged) {
            r->per_dir_config = cache->per_dir_result;
            familiar = 1;
        }

        if (familiar) {
            apr_finfo_t thisinfo;
            int res;
            allow_options_t opts;
            core_dir_config *this_dir;

            this_dir = ap_get_core_module_config(r->per_dir_config);
            opts = this_dir->opts;
             
            if (!(opts & OPT_SYM_LINKS)) {
                rv = ap_run_dirwalk_stat(&thisinfo, r,
                                         APR_FINFO_MIN | APR_FINFO_NAME | APR_FINFO_LINK);
                 
                if ((rv != APR_INCOMPLETE) && (rv != APR_SUCCESS)) {
                     
                    ap_log_rerror(APLOG_MARK, APLOG_ERR, rv, r, APLOGNO(00031)
                                  "access to %s failed; stat of '%s' failed.",
                                  r->uri, r->filename);
                    return r->status = HTTP_FORBIDDEN;
                }
                if (thisinfo.filetype == APR_LNK) {
                     
                    if ((res = resolve_symlink(r->filename, &thisinfo,
                                               opts, r->pool)) != OK) {
                        ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r, APLOGNO(00032)
                                      "Symbolic link not allowed "
                                      "or link target not accessible: %s",
                                      r->filename);
                        return r->status = res;
                    }
                }
            }
            return OK;
        }

        if (cache->walked->nelts) {
            now_merged = ((walk_walked_t*)cache->walked->elts)
                [cache->walked->nelts - 1].merged;
        }
    }
    else {
         
        int sec_idx;
        int matches = cache->walked->nelts;
        int cached_matches = matches;
        walk_walked_t *last_walk = (walk_walked_t*)cache->walked->elts;
        core_dir_config *this_dir;
        core_opts_t opts;
        apr_finfo_t thisinfo;
        char *save_path_info;
        apr_size_t buflen;
        char *buf;
        unsigned int seg, startseg;
        apr_pool_t *rxpool = NULL;

         
        apr_size_t filename_len;
#ifdef CASE_BLIND_FILESYSTEM
        apr_size_t canonical_len;
#endif

        cached &= auth_internal_per_conf;

         
        this_dir = ap_get_core_module_config(r->per_dir_config);
        opts.opts = this_dir->opts;
        opts.add = this_dir->opts_add;
        opts.remove = this_dir->opts_remove;
        opts.override = this_dir->override;
        opts.override_opts = this_dir->override_opts;
        opts.override_list = this_dir->override_list;

         
        if ((r->finfo.filetype == APR_DIR) && r->path_info && *r->path_info)
        {
            if ((rv = apr_filepath_merge(&r->path_info, r->filename,
                                         r->path_info,
                                         APR_FILEPATH_NOTABOVEROOT, r->pool))
                != APR_SUCCESS) {
                ap_log_rerror(APLOG_MARK, APLOG_ERR, rv, r, APLOGNO(00033)
                              "dir_walk error, path_info %s is not relative "
                              "to the filename path %s for uri %s",
                              r->path_info, r->filename, r->uri);
                return HTTP_INTERNAL_SERVER_ERROR;
            }

            save_path_info = NULL;
        }
        else {
            save_path_info = r->path_info;
            r->path_info = r->filename;
        }

#ifdef CASE_BLIND_FILESYSTEM

        canonical_len = 0;
        while (r->canonical_filename && r->canonical_filename[canonical_len]
               && (r->canonical_filename[canonical_len]
                   == r->path_info[canonical_len])) {
             ++canonical_len;
        }

        while (canonical_len
               && ((r->canonical_filename[canonical_len - 1] != '/'
                   && r->canonical_filename[canonical_len - 1])
                   || (r->path_info[canonical_len - 1] != '/'
                       && r->path_info[canonical_len - 1]))) {
            --canonical_len;
        }

         
        rv = apr_filepath_root((const char **)&r->filename,
                               (const char **)&r->path_info,
                               canonical_len ? 0 : APR_FILEPATH_TRUENAME,
                               r->pool);
        filename_len = strlen(r->filename);

         
        if ((rv == APR_SUCCESS) && canonical_len
            && (filename_len > canonical_len)) {
            rv = apr_filepath_root((const char **)&r->filename,
                                   (const char **)&r->path_info,
                                   APR_FILEPATH_TRUENAME, r->pool);
            filename_len = strlen(r->filename);
            canonical_len = 0;
        }

#else  

        rv = apr_filepath_root((const char **)&r->filename,
                               (const char **)&r->path_info,
                               0, r->pool);
        filename_len = strlen(r->filename);

#endif

        if (rv != APR_SUCCESS) {
            ap_log_rerror(APLOG_MARK, APLOG_ERR, rv, r, APLOGNO(00034)
                          "dir_walk error, could not determine the root "
                          "path of filename %s%s for uri %s",
                          r->filename, r->path_info, r->uri);
            return HTTP_INTERNAL_SERVER_ERROR;
        }

         
        buflen = filename_len + strlen(r->path_info) + 2;
        buf = apr_palloc(r->pool, buflen);
        memcpy(buf, r->filename, filename_len + 1);
        r->filename = buf;
        thisinfo.valid = APR_FINFO_TYPE;
        thisinfo.filetype = APR_DIR;  

         
        startseg = seg = ap_count_dirs(r->filename);
        sec_idx = 0;

         
        do {
            int res;
            char *seg_name;
            char *delim;
            int temp_slash=0;

             
            if ((seg > startseg) && r->filename[filename_len-1] != '/') {
                r->filename[filename_len++] = '/';
                r->filename[filename_len] = 0;
                temp_slash=1;
            }

             
            for (; sec_idx < num_sec; ++sec_idx) {

                ap_conf_vector_t *entry_config = sec_ent[sec_idx];
                core_dir_config *entry_core;
                entry_core = ap_get_core_module_config(entry_config);

                 
                if (entry_core->r || entry_core->d_components > seg) {
                    break;
                }

                 
                if (entry_core->d_components
                    && ((entry_core->d_components < seg)
                     || (entry_core->d_is_fnmatch
                         ? (apr_fnmatch(entry_core->d, r->filename,
                                        APR_FNM_PATHNAME) != APR_SUCCESS)
                         : (strcmp(r->filename, entry_core->d) != 0)))) {
                    continue;
                }

                 
                core_opts_merge(sec_ent[sec_idx], &opts);

                 
                if (matches) {
                    if (last_walk->matched == sec_ent[sec_idx]) {
                        now_merged = last_walk->merged;
                        ++last_walk;
                        --matches;
                        continue;
                    }

                     
                    cache->walked->nelts -= matches;
                    matches = 0;
                    cached = 0;
                }

                if (now_merged) {
                    now_merged = ap_merge_per_dir_configs(r->pool,
                                                          now_merged,
                                                          sec_ent[sec_idx]);
                }
                else {
                    now_merged = sec_ent[sec_idx];
                }

                last_walk = (walk_walked_t*)apr_array_push(cache->walked);
                last_walk->matched = sec_ent[sec_idx];
                last_walk->merged = now_merged;
            }

             
            do {   

                ap_conf_vector_t *htaccess_conf = NULL;

                 
                if (seg < startseg || (!opts.override && opts.override_list == NULL)) {
                    break;
                }


                res = ap_parse_htaccess(&htaccess_conf, r, opts.override,
                                        opts.override_opts, opts.override_list,
                                        apr_pstrdup(r->pool, r->filename),
                                        sconf->access_name);
                if (res) {
                    return res;
                }

                if (!htaccess_conf) {
                    break;
                }

                 
                core_opts_merge(htaccess_conf, &opts);

                 
                if (matches) {
                    if (last_walk->matched == htaccess_conf) {
                        now_merged = last_walk->merged;
                        ++last_walk;
                        --matches;
                        break;
                    }

                     
                    cache->walked->nelts -= matches;
                    matches = 0;
                    cached = 0;
                }

                if (now_merged) {
                    now_merged = ap_merge_per_dir_configs(r->pool,
                                                          now_merged,
                                                          htaccess_conf);
                }
                else {
                    now_merged = htaccess_conf;
                }

                last_walk = (walk_walked_t*)apr_array_push(cache->walked);
                last_walk->matched = htaccess_conf;
                last_walk->merged = now_merged;

            } while (0);  

             
            if (temp_slash) {
                r->filename[--filename_len] = '\0';
            }

             
            if (!r->path_info || !*r->path_info) {
                break;
            }

             

            seg_name = r->filename + filename_len;
            delim = strchr(r->path_info + (*r->path_info == '/' ? 1 : 0), '/');
            if (delim) {
                apr_size_t path_info_len = delim - r->path_info;
                *delim = '\0';
                memcpy(seg_name, r->path_info, path_info_len + 1);
                filename_len += path_info_len;
                r->path_info = delim;
                *delim = '/';
            }
            else {
                apr_size_t path_info_len = strlen(r->path_info);
                memcpy(seg_name, r->path_info, path_info_len + 1);
                filename_len += path_info_len;
                r->path_info += path_info_len;
            }
            if (*seg_name == '/')
                ++seg_name;

             
            if (!*seg_name) {
                break;
            }

             
            if (r->finfo.filetype != APR_NOFILE
#ifdef CASE_BLIND_FILESYSTEM
                && (filename_len <= canonical_len)
#endif
                && ((opts.opts & (OPT_SYM_OWNER | OPT_SYM_LINKS)) == OPT_SYM_LINKS))
            {

                thisinfo.filetype = APR_DIR;
                ++seg;
                continue;
            }

             
            rv = ap_run_dirwalk_stat(&thisinfo, r,
                                     APR_FINFO_MIN | APR_FINFO_NAME | APR_FINFO_LINK);

            if (APR_STATUS_IS_ENOENT(rv)) {
                 
                thisinfo.filetype = APR_NOFILE;
                break;
            }
            else if (APR_STATUS_IS_EACCES(rv)) {
                ap_log_rerror(APLOG_MARK, APLOG_ERR, rv, r, APLOGNO(00035)
                              "access to %s denied (filesystem path '%s') "
                              "because search permissions are missing on a "
                              "component of the path", r->uri, r->filename);
                return r->status = HTTP_FORBIDDEN;
            }
            else if ((rv != APR_SUCCESS && rv != APR_INCOMPLETE)
                     || !(thisinfo.valid & APR_FINFO_TYPE)) {
                 
                ap_log_rerror(APLOG_MARK, APLOG_ERR, rv, r, APLOGNO(00036)
                              "access to %s failed (filesystem path '%s')", 
                              r->uri, r->filename);
                return r->status = HTTP_FORBIDDEN;
            }

             
            if ((thisinfo.valid & APR_FINFO_NAME)
                && strcmp(seg_name, thisinfo.name)) {
                 
                strcpy(seg_name, thisinfo.name);
                filename_len = strlen(r->filename);
            }

            if (thisinfo.filetype == APR_LNK) {
                 
                if ((res = resolve_symlink(r->filename, &thisinfo,
                                           opts.opts, r->pool)) != OK) {
                    ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r, APLOGNO(00037)
                                  "Symbolic link not allowed "
                                  "or link target not accessible: %s",
                                  r->filename);
                    return r->status = res;
                }
            }

             
            if (thisinfo.filetype == APR_REG ||
                thisinfo.filetype == APR_NOFILE) {
                 
                break;
            }
            else if (thisinfo.filetype != APR_DIR) {
                ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r, APLOGNO(00038)
                              "Forbidden: %s doesn't point to "
                              "a file or directory",
                              r->filename);
                return r->status = HTTP_FORBIDDEN;
            }

            ++seg;
        } while (thisinfo.filetype == APR_DIR);

         
        if (r->finfo.filetype == APR_NOFILE || r->finfo.filetype == APR_LNK) {
            r->finfo = thisinfo;
        }

         
        if (save_path_info) {
            if (r->path_info && *r->path_info) {
                r->path_info = ap_make_full_path(r->pool, r->path_info,
                                                 save_path_info);
            }
            else {
                r->path_info = save_path_info;
            }
        }

         
        for (; sec_idx < num_sec; ++sec_idx) {

            int nmatch = 0;
            int i;
            ap_regmatch_t *pmatch = NULL;

            core_dir_config *entry_core;
            entry_core = ap_get_core_module_config(sec_ent[sec_idx]);

            if (!entry_core->r) {
                continue;
            }

            if (entry_core->refs && entry_core->refs->nelts) {
                if (!rxpool) {
                    apr_pool_create(&rxpool, r->pool);
                }
                nmatch = entry_core->refs->nelts;
                pmatch = apr_palloc(rxpool, nmatch*sizeof(ap_regmatch_t));
            }

            if (ap_regexec(entry_core->r, r->filename, nmatch, pmatch, 0)) {
                continue;
            }

            for (i = 0; i < nmatch; i++) {
                if (pmatch[i].rm_so >= 0 && pmatch[i].rm_eo >= 0 &&
                    ((const char **)entry_core->refs->elts)[i]) {
                    apr_table_setn(r->subprocess_env, 
                                   ((const char **)entry_core->refs->elts)[i],
                                   apr_pstrndup(r->pool,
                                   r->filename + pmatch[i].rm_so,
                                   pmatch[i].rm_eo - pmatch[i].rm_so));
                }
            }

             
            core_opts_merge(sec_ent[sec_idx], &opts);

             
            if (matches) {
                if (last_walk->matched == sec_ent[sec_idx]) {
                    now_merged = last_walk->merged;
                    ++last_walk;
                    --matches;
                    continue;
                }

                 
                cache->walked->nelts -= matches;
                matches = 0;
                cached = 0;
            }

            if (now_merged) {
                now_merged = ap_merge_per_dir_configs(r->pool,
                                                      now_merged,
                                                      sec_ent[sec_idx]);
            }
            else {
                now_merged = sec_ent[sec_idx];
            }

            last_walk = (walk_walked_t*)apr_array_push(cache->walked);
            last_walk->matched = sec_ent[sec_idx];
            last_walk->merged = now_merged;
        }

        if (rxpool) {
            apr_pool_destroy(rxpool);
        }

         
        if (matches) {
            cache->walked->nelts -= matches;
            cached = 0;
        }
        else if (cache->walked->nelts > cached_matches) {
            cached = 0;
        }
    }

 

     
    r->canonical_filename = r->filename;

    if (r->finfo.filetype == APR_DIR) {
        cache->cached = r->filename;
    }
    else {
        cache->cached = ap_make_dirstr_parent(r->pool, r->filename);
    }

    if (cached
        && r->per_dir_config == cache->dir_conf_merged) {
        r->per_dir_config = cache->per_dir_result;
        return OK;
    }

    cache->dir_conf_tested = sec_ent;
    cache->dir_conf_merged = r->per_dir_config;

     
    if (now_merged) {
        r->per_dir_config = ap_merge_per_dir_configs(r->pool,
                                                     r->per_dir_config,
                                                     now_merged);
    }
    cache->per_dir_result = r->per_dir_config;

    return OK;
}
