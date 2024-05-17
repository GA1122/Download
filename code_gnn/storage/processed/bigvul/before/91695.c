int am_check_permissions(request_rec *r, am_cache_entry_t *session)
{
    am_dir_cfg_rec *dir_cfg;
    int i, j;
    int skip_or = 0;
    const apr_array_header_t *backrefs = NULL;

    dir_cfg = am_get_dir_cfg(r);

     
    for (i = 0; i < dir_cfg->cond->nelts; i++) {
        const am_cond_t *ce;
        const char *value = NULL;
        int match = 0;

        ce = &((am_cond_t *)(dir_cfg->cond->elts))[i];

        am_diag_printf(r, "%s processing condition %d of %d: %s ",
                       __func__, i, dir_cfg->cond->nelts,
                       am_diag_cond_str(r, ce));

         
        if (ce->flags & AM_COND_FLAG_IGN)
            continue;

         
        if (skip_or) {
            if (!(ce->flags & AM_COND_FLAG_OR))
                skip_or = 0;

            ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r,
                          "Skip %s, [OR] rule matched previously",
                          ce->directive);

            am_diag_printf(r, "Skip, [OR] rule matched previously\n");
            continue;
        }
        
         
        for (j = 0; (j < session->size) && !match; j++) {
            const char *varname = NULL;
            am_envattr_conf_t *envattr_conf = NULL;

             
            if (ce->flags & AM_COND_FLAG_MAP) {
                envattr_conf =  (am_envattr_conf_t *)apr_hash_get(dir_cfg->envattr, 
                                         am_cache_entry_get_string(session,&session->env[j].varname),
                                         APR_HASH_KEY_STRING);
                                                    
                if (envattr_conf != NULL)
                    varname = envattr_conf->name;
            }

             
            if (varname == NULL)
                varname = am_cache_entry_get_string(session,
                                                    &session->env[j].varname);
                      
            if (strcmp(varname, ce->varname) != 0)
                    continue;

            value = am_cache_entry_get_string(session, &session->env[j].value);

             
            if (ce->flags & AM_COND_FLAG_FSTR)
                ce = am_cond_substitue(r, ce, backrefs);

            ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r,
                          "Evaluate %s vs \"%s\"", 
                          ce->directive, value);

            am_diag_printf(r, "evaluate value \"%s\" ", value);
    
            if (value == NULL) {
                 match = 0;           

            } else if (ce->flags & (AM_COND_FLAG_REG|AM_COND_FLAG_REF)) {
                 int nsub = ce->regex->re_nsub + 1;
                 ap_regmatch_t *regmatch;

                 regmatch = (ap_regmatch_t *)apr_palloc(r->pool, 
                            nsub * sizeof(*regmatch));

                 match = !ap_regexec(ce->regex, value, nsub, regmatch, 0);
                 if (match)
                     backrefs = am_cond_backrefs(r, ce, value, regmatch);

            } else if (ce->flags & AM_COND_FLAG_REG) {
                 match = !ap_regexec(ce->regex, value, 0, NULL, 0);

            } else if (ce->flags & (AM_COND_FLAG_SUB|AM_COND_FLAG_NC)) {
                 match = (ap_strcasestr(ce->str, value) != NULL);

            } else if (ce->flags & AM_COND_FLAG_SUB) {
                 match = (strstr(ce->str, value) != NULL);

            } else if (ce->flags & AM_COND_FLAG_NC) {
                 match = !strcasecmp(ce->str, value);

            } else {
                 match = !strcmp(ce->str, value);
            }

        am_diag_printf(r, "match=%s, ", match ? "yes" : "no");
        }

        if (ce->flags & AM_COND_FLAG_NOT) {
            match = !match;

            am_diag_printf(r, "negating now match=%s ", match ? "yes" : "no");
        }

        ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r,
                      "%s: %smatch", ce->directive,
                      (match == 0) ? "no ": "");

         
        if (!match & !(ce->flags & AM_COND_FLAG_OR)) {
            ap_log_rerror(APLOG_MARK, APLOG_NOTICE, 0, r,
                          "Client failed to match %s",
                          ce->directive);

            am_diag_printf(r, "failed (no OR condition)"
                           " returning HTTP_FORBIDDEN\n");
            return HTTP_FORBIDDEN;
        }

         
        if (match && (ce->flags & AM_COND_FLAG_OR))
            skip_or = 1;

        am_diag_printf(r, "\n");
    }

    am_diag_printf(r, "%s succeeds\n", __func__);

    return OK;
}
