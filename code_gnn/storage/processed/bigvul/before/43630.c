static APR_INLINE int check_headers(request_rec *r)
{
    const char *loc;
    struct check_header_ctx ctx = { 0, 0 };

    ctx.r = r;
    apr_table_do(check_header, &ctx, r->headers_out, NULL);
    if (ctx.error)
        return 0;  

    if ((loc = apr_table_get(r->headers_out, "Location")) != NULL) {
        const char *scheme_end = ap_strchr_c(loc, ':');

         
        if (!ap_is_url(loc))
            goto bad;

        if (scheme_end[1] != '/' || scheme_end[2] != '/')
            goto bad;
    }

    return 1;

bad:
    ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r, APLOGNO(02431)
                  "Bad Location header in response: '%s', aborting request",
                  loc);
    return 0;
}