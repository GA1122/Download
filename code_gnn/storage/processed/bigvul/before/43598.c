AP_DECLARE(int) ap_file_walk(request_rec *r)
{
    ap_conf_vector_t *now_merged = NULL;
    core_dir_config *dconf = ap_get_core_module_config(r->per_dir_config);
    ap_conf_vector_t **sec_ent = NULL;
    int num_sec = 0;
    walk_cache_t *cache;
    const char *test_file;
    int cached;

    if (dconf->sec_file) {
        sec_ent = (ap_conf_vector_t **)dconf->sec_file->elts;
        num_sec = dconf->sec_file->nelts;
    }

     
    if (r->filename == NULL) {
        return OK;
    }

    cache = prep_walk_cache(AP_NOTE_FILE_WALK, r);
    cached = (cache->cached != NULL);

     
    if (!num_sec) {
        return OK;
    }

     
    test_file = strrchr(r->filename, '/');
    if (test_file == NULL) {
        test_file = apr_pstrdup(r->pool, r->filename);
    }
    else {
        test_file = apr_pstrdup(r->pool, ++test_file);
    }

     
    if (cached
        && (cache->dir_conf_tested == sec_ent)
        && (strcmp(test_file, cache->cached) == 0)) {
         
        if (r->per_dir_config == cache->per_dir_result) {
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
        apr_pool_t *rxpool = NULL;

        cached &= auth_internal_per_conf;
        cache->cached = test_file;

         
        for (sec_idx = 0; sec_idx < num_sec; ++sec_idx) {
            core_dir_config *entry_core;
            entry_core = ap_get_core_module_config(sec_ent[sec_idx]);

            if (entry_core->r) {

                int nmatch = 0;
                int i;
                ap_regmatch_t *pmatch = NULL;

                if (entry_core->refs && entry_core->refs->nelts) {
                    if (!rxpool) {
                        apr_pool_create(&rxpool, r->pool);
                    }
                    nmatch = entry_core->refs->nelts;
                    pmatch = apr_palloc(rxpool, nmatch*sizeof(ap_regmatch_t));
                }

                if (ap_regexec(entry_core->r, cache->cached, nmatch, pmatch, 0)) {
                    continue;
                }

                for (i = 0; i < nmatch; i++) {
                    if (pmatch[i].rm_so >= 0 && pmatch[i].rm_eo >= 0 && 
                        ((const char **)entry_core->refs->elts)[i]) {
                        apr_table_setn(r->subprocess_env,
                                       ((const char **)entry_core->refs->elts)[i],
                                       apr_pstrndup(r->pool,
                                       cache->cached + pmatch[i].rm_so,
                                       pmatch[i].rm_eo - pmatch[i].rm_so));
                    }
                }

            }
            else {
                if ((entry_core->d_is_fnmatch
                       ? apr_fnmatch(entry_core->d, cache->cached, APR_FNM_PATHNAME)
                       : strcmp(entry_core->d, cache->cached))) {
                    continue;
                }
            }

             
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
