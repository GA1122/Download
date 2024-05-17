gs_setdefaultcmykicc(const gs_gstate * pgs, gs_param_string * pval)
{
    int code;
    char* pname;
    int namelen = (pval->size)+1;
    gs_memory_t *mem = pgs->memory;

    pname = (char *)gs_alloc_bytes(mem, namelen,
                             "set_default_cmyk_icc");
    if (pname == NULL)
        return_error(gs_error_VMerror);
    memcpy(pname,pval->data,namelen-1);
    pname[namelen-1] = 0;
    code = gsicc_set_profile(pgs->icc_manager,
        (const char*) pname, namelen, DEFAULT_CMYK);
    gs_free_object(mem, pname,
                "set_default_cmyk_icc");
    if (code < 0)
        return gs_throw(code, "cannot find default cmyk icc profile");
    return code;
}