static int resolve_symlink(char *d, apr_finfo_t *lfi, int opts, apr_pool_t *p)
{
    apr_finfo_t fi;
    const char *savename;

    if (!(opts & (OPT_SYM_OWNER | OPT_SYM_LINKS))) {
        return HTTP_FORBIDDEN;
    }

     
    savename = (lfi->valid & APR_FINFO_NAME) ? lfi->name : NULL;

     
    if (!(opts & OPT_SYM_OWNER)) {
        if (apr_stat(&fi, d, lfi->valid & ~(APR_FINFO_NAME | APR_FINFO_LINK), p)
            != APR_SUCCESS)
        {
            return HTTP_FORBIDDEN;
        }

         
        memcpy(lfi, &fi, sizeof(fi));
        if (savename) {
            lfi->name = savename;
            lfi->valid |= APR_FINFO_NAME;
        }

        return OK;
    }

     
    if (!(lfi->valid & APR_FINFO_OWNER)) {
        if (apr_stat(lfi, d, lfi->valid | APR_FINFO_LINK | APR_FINFO_OWNER, p)
            != APR_SUCCESS)
        {
            return HTTP_FORBIDDEN;
        }
    }

    if (apr_stat(&fi, d, lfi->valid & ~(APR_FINFO_NAME), p) != APR_SUCCESS) {
        return HTTP_FORBIDDEN;
    }

    if (apr_uid_compare(fi.user, lfi->user) != APR_SUCCESS) {
        return HTTP_FORBIDDEN;
    }

     
    memcpy(lfi, &fi, sizeof(fi));
    if (savename) {
        lfi->name = savename;
        lfi->valid |= APR_FINFO_NAME;
    }

    return OK;
}
