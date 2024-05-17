const am_cond_t *am_cond_substitue(request_rec *r, const am_cond_t *ce, 
                                   const apr_array_header_t *backrefs)
{
    am_cond_t *c;
    const char *instr = ce->str;
    apr_size_t inlen = strlen(instr);
    const char *outstr = "";
    size_t last;
    size_t i;

    c = (am_cond_t *)apr_pmemdup(r->pool, ce, sizeof(*ce));
    last = 0;

    for (i = strcspn(instr, "%"); i < inlen; i += strcspn(instr + i, "%")) {
        const char *fstr;
        const char *ns;
        const char *name;
        const char *value;
        apr_size_t flen;
        apr_size_t pad;
        apr_size_t nslen;

         
	assert(instr[i] == '%');

         
        fstr = instr + i + 1;
        if (*fstr == '{') {           
            pad = 3;  
            fstr++;
            flen = strcspn(fstr, "}");

             
            if (fstr[flen] == '\0') {
                pad = 2;  
                i += flen + pad;
                break;
            }

        } else if (*fstr == '\0') {   
            break;

        } else {                      
            pad = 1;  
            flen = 1;
        }

          
        fstr = apr_pstrndup(r->pool, fstr, flen);
        if ((nslen = strcspn(fstr, ":")) != flen) {
            ns = apr_pstrndup(r->pool, fstr, nslen);
            name = fstr + nslen + 1;  
        } else {
            nslen = 0;
            ns = "";
            name = fstr;
        }

        value = NULL;
        if ((*ns == '\0') && (strspn(fstr, "0123456789") == flen) && (backrefs != NULL)) {
             
            int d = (int)apr_atoi64(fstr);

            if ((d >= 0) && (d < backrefs->nelts)) 
                value = ((const char **)(backrefs->elts))[d];

        } else if ((*ns == '\0') && (strcmp(fstr, "%") == 0)) {
             
            value = fstr;

        } else if (strcmp(ns, "ENV") == 0) {
             
            value = apr_table_get(r->subprocess_env, name);
            if (value == NULL) {
                value = apr_table_get(r->notes, name);
            }
            if (value == NULL) {
                value = getenv(name);
            }

            ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r,
                          "Resolving \"%s\" from ENV to \"%s\"",
                          name, value == NULL ? "(nothing)" : value);
        }

         
         if (value == NULL)
            value = "";

         
        outstr = apr_pstrcat(r->pool, outstr,
                             apr_pstrndup(r->pool, instr + last, i - last), 
                             value, NULL);
        last = i + flen + pad;

         
        i += flen + pad;
    }

     
    outstr = apr_pstrcat(r->pool, outstr,
                         apr_pstrndup(r->pool, instr + last, i - last), 
                         NULL);
    c->str = outstr;

    ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r,
                  "Directive %s, \"%s\" substituted into \"%s\"",
                  ce->directive, instr, outstr);

     
    if (ce->flags & AM_COND_FLAG_REG) {
        int regex_flags = AP_REG_EXTENDED|AP_REG_NOSUB;
 
        if (ce->flags & AM_COND_FLAG_NC)
            regex_flags |= AP_REG_ICASE;
 
        c->regex = ap_pregcomp(r->pool, outstr, regex_flags);
        if (c->regex == NULL) {
             AM_LOG_RERROR(APLOG_MARK, APLOG_WARNING, 0, r,
                           "Invalid regular expression \"%s\"", outstr);
             return ce;
        }
    }

    return (const am_cond_t *)c;
}
