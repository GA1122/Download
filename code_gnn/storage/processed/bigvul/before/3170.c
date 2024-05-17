static int validateseparationspace(i_ctx_t * i_ctx_p, ref **space)
{
    int code = 0;
    ref *sepspace = *space;
    ref nameref, sref, sname, altspace, tref;

    if (!r_is_array(sepspace))
        return_error(gs_error_typecheck);
     
    if (r_size(sepspace) != 4)
        return_error(gs_error_rangecheck);

     
    code = array_get(imemory, sepspace, 1, &sname);
    if (code < 0)
        return code;
    if (!r_has_type(&sname, t_name)) {
        if (!r_has_type(&sname, t_string))
            return_error(gs_error_typecheck);
        else {
            code = name_from_string(imemory, &sname, &sname);
            if (code < 0)
                return code;
        }
    }

     
    code = array_get(imemory, sepspace, 3, &tref);
    if (code < 0)
        return code;
    check_proc(tref);

     
    code = array_get(imemory, sepspace, 2, &altspace);
    if (code < 0)
        return code;
    if (r_has_type(&altspace, t_name))
        ref_assign(&nameref, &altspace);
    else {
         
        if (!r_is_array(&altspace))
            return_error(gs_error_typecheck);
         
        code = array_get(imemory, &altspace, 0, &tref);
        if (code < 0)
            return code;
        if (!r_has_type(&tref, t_name))
            return_error(gs_error_typecheck);
        ref_assign(&nameref, &tref);
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
