static int validatedevicenspace(i_ctx_t * i_ctx_p, ref **space)
{
    int i, code = 0;
    ref *devicenspace = *space, proc;
    ref nameref, sref, altspace, namesarray, sname;

     
    if (r_size(devicenspace) < 4)
        return_error(gs_error_rangecheck);
     
    code = array_get(imemory, devicenspace, 1, &namesarray);
    if (code < 0)
        return code;
    if (!r_is_array(&namesarray))
        return_error(gs_error_typecheck);
     
    if (r_size(&namesarray) < 1)
        return_error(gs_error_typecheck);
     
    if (r_size(&namesarray) > MAX_COMPONENTS_IN_DEVN)     
        return_error(gs_error_limitcheck);
     
    code = array_get(imemory, devicenspace, 3, &proc);
    if (code < 0)
        return code;
    check_proc(proc);

     
    for (i = 0; i < r_size(&namesarray); ++i) {
        array_get(imemory, &namesarray, (long)i, &sname);
        switch (r_type(&sname)) {
                case t_string:
                case t_name:
                    break;
                default:
                    return_error(gs_error_typecheck);
        }
    }

     
    code = array_get(imemory, devicenspace, 2, &altspace);
    if (code < 0)
        return code;
    if (r_has_type(&altspace, t_name))
        ref_assign(&nameref, &altspace);
    else {
         
        if (!r_is_array(&altspace))
            return_error(gs_error_typecheck);
         
        code = array_get(imemory, &altspace, 0, &nameref);
        if (code < 0)
            return code;
        if (!r_has_type(&nameref, t_name))
            return_error(gs_error_typecheck);
    }
     
    name_string_ref(imemory, &nameref, &sref);
     
    if (r_size(&sref) == 7) {
        if (strncmp((const char *)sref.value.const_bytes, "Indexed", 7) == 0)
            return_error(gs_error_typecheck);
        if (strncmp((const char *)sref.value.const_bytes, "Pattern", 7) == 0)
            return_error(gs_error_typecheck);
        if (strncmp((const char *)sref.value.const_bytes, "DeviceN", 7) == 0)
            return_error(gs_error_typecheck);
    }
     
    if (r_size(&sref) == 9 && strncmp((const char *)sref.value.const_bytes, "Separation", 9) == 0)
           return_error(gs_error_typecheck);

    ref_assign(*space, &altspace);
    return 0;
}
