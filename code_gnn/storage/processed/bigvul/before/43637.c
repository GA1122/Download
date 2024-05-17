static apr_status_t validate_status_line(request_rec *r)
{
    char *end;

    if (r->status_line) {
        int len = strlen(r->status_line);
        if (len < 3
            || apr_strtoi64(r->status_line, &end, 10) != r->status
            || (end - 3) != r->status_line
            || (len >= 4 && ! apr_isspace(r->status_line[3]))) {
            r->status_line = NULL;
            return APR_EGENERAL;
        }
         
        if (len == 3) {
            r->status_line = apr_pstrcat(r->pool, r->status_line, " ", NULL);
            return APR_EGENERAL;
        }
        return APR_SUCCESS;
    }
    return APR_EGENERAL;
}