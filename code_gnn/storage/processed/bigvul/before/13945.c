gs_setdevicenprofileicc(const gs_gstate * pgs, gs_param_string * pval)
{
    int code = 0;
    char *pname, *pstr, *pstrend, *last = NULL;
    int namelen = (pval->size)+1;
    gs_memory_t *mem = pgs->memory;

     
    if (pval->size != 0) {
         
        pname = (char *)gs_alloc_bytes(mem, namelen,
                                     "set_devicen_profile_icc");
        if (pname == NULL)
            return_error(gs_error_VMerror);
        memcpy(pname,pval->data,namelen-1);
        pname[namelen-1] = 0;
        pstr = gs_strtok(pname, ",;", &last);
        while (pstr != NULL) {
            namelen = strlen(pstr);
             
            while ( namelen > 0 && pstr[0] == 0x20) {
                pstr++;
                namelen--;
            }
            namelen = strlen(pstr);
            pstrend = &(pstr[namelen-1]);
            while ( namelen > 0 && pstrend[0] == 0x20) {
                pstrend--;
                namelen--;
            }
            code = gsicc_set_profile(pgs->icc_manager, (const char*) pstr, namelen, DEVICEN_TYPE);
            if (code < 0)
                return gs_throw(code, "cannot find devicen icc profile");
            pstr = gs_strtok(NULL, ",;", &last);
        }
        gs_free_object(mem, pname,
        "set_devicen_profile_icc");
        return code;
    }
    return 0;
}
