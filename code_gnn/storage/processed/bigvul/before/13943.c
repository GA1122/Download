gs_setdefaultgrayicc(const gs_gstate * pgs, gs_param_string * pval)
{
    int code;
    char *pname;
    int namelen = (pval->size)+1;
    gs_memory_t *mem = pgs->memory;
    bool not_initialized;

     
    not_initialized = (pgs->icc_manager->default_gray == NULL);

    pname = (char *)gs_alloc_bytes(mem, namelen,
                             "set_default_gray_icc");
    if (pname == NULL)
        return_error(gs_error_VMerror);
    memcpy(pname,pval->data,namelen-1);
    pname[namelen-1] = 0;
    code = gsicc_set_profile(pgs->icc_manager,
        (const char*) pname, namelen, DEFAULT_GRAY);
    gs_free_object(mem, pname,
        "set_default_gray_icc");
    if (code < 0)
        return gs_throw(code, "cannot find default gray icc profile");
     
    if (not_initialized) {
        code = gsicc_init_gs_colors((gs_gstate*) pgs);
    }
    if (code < 0)
        return gs_throw(code, "error initializing gstate color spaces to icc");
    return code;
}
