gs_seticcdirectory(const gs_gstate * pgs, gs_param_string * pval)
{
    char *pname;
    int namelen = (pval->size)+1;
    const gs_memory_t *mem = pgs->memory;

     
    if (pval->size != 0 ) {
        pname = (char *)gs_alloc_bytes((gs_memory_t *)mem, namelen,
                                       "set_icc_directory");
        if (pname == NULL)
            return gs_rethrow(-1, "cannot allocate directory name");
        memcpy(pname,pval->data,namelen-1);
        pname[namelen-1] = 0;
        gs_lib_ctx_set_icc_directory(mem, (const char*) pname, namelen);
        gs_free_object((gs_memory_t *)mem, pname, "set_icc_directory");
    }
    return 0;
}
