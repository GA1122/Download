gs_setsrcgtagicc(const gs_gstate * pgs, gs_param_string * pval)
{
    int code;
    char *pname;
    int namelen = (pval->size)+1;
    gs_memory_t *mem = pgs->memory;

    if (pval->size == 0) return 0;
    pname = (char *)gs_alloc_bytes(mem, namelen, "set_srcgtag_icc");
    if (pname == NULL)
        return_error(gs_error_VMerror);
    memcpy(pname,pval->data,namelen-1);
    pname[namelen-1] = 0;
    code = gsicc_set_srcgtag_struct(pgs->icc_manager, (const char*) pname,
                                   namelen);
    gs_free_object(mem, pname, "set_srcgtag_icc");
    if (code < 0)
        return gs_rethrow(code, "cannot find srctag file");
    return code;
}
