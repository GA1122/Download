AP_DECLARE(int) ap_if_walk(request_rec *r)
{
    ap_conf_vector_t *now_merged = NULL;
    core_dir_config *dconf = ap_get_core_module_config(r->per_dir_config);
    ap_conf_vector_t **sec_ent = NULL;
    int num_sec = 0;
    walk_cache_t *cache;
    int cached;
    int sec_idx;
    int matches;
    int cached_matches;
    int prev_result = -1;
    walk_walked_t *last_walk;

    if (dconf->sec_if) {
        sec_ent = (ap_conf_vector_t **)dconf->sec_if->elts;
        num_sec = dconf->sec_if->nelts;
    }

     
    if (!num_sec) {
        return OK;
    }

    cache = prep_walk_cache(AP_NOTE_IF_WALK, r);
    cached = (cache->cached != NULL);
    cache->cached = (void *)1;
    matches = cache->walked->nelts;
    cached_matches = matches;
    last_walk = (walk_walked_t*)cache->walked->elts;

    cached &= auth_internal_per_conf;

     
    for (sec_idx = 0; sec_idx < num_sec; ++sec_idx) {
        const char *err = NULL;
        core_dir_config *entry_core;
        int rc;
        entry_core = ap_get_core_module_config(sec_ent[sec_idx]);

        AP_DEBUG_ASSERT(entry_core->condition_ifelse != 0);
        if (entry_core->condition_ifelse & AP_CONDITION_ELSE) {
            AP_DEBUG_ASSERT(prev_result != -1);
            if (prev_result == 1)
                continue;
        }

        if (entry_core->condition_ifelse & AP_CONDITION_IF) {
            rc = ap_expr_exec(r, entry_core->condition, &err);
            if (rc <= 0) {
                if (rc < 0)
                    ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r, APLOGNO(00039)
                                  "Failed to evaluate <If > condition: %s",
                                  err);
                prev_result = 0;
                continue;
            }
            prev_result = 1;
        }
        else {
            prev_result = -1;
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

     
    if (matches) {
        cache->walked->nelts -= matches;
        cached = 0;
    }
    else if (cache->walked->nelts > cached_matches) {
        cached = 0;
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
