gs_setnamedprofileicc(const gs_gstate * pgs, gs_param_string * pval)
{
    int code;
    char* pname;
    int namelen = (pval->size)+1;
    gs_memory_t *mem = pgs->memory;

     
    if (pval->size != 0) {
        pname = (char *)gs_alloc_bytes(mem, namelen,
                                 "set_named_profile_icc");
        if (pname == NULL)
            return_error(gs_error_VMerror);
        memcpy(pname,pval->data,namelen-1);
        pname[namelen-1] = 0;
        code = gsicc_set_profile(pgs->icc_manager,
            (const char*) pname, namelen, NAMED_TYPE);
        gs_free_object(mem, pname,
                "set_named_profile_icc");
        if (code < 0)
            return gs_rethrow(code, "cannot find named color icc profile");
        return code;
    }
    return 0;
}
