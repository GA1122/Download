static int validateiccspace(i_ctx_t * i_ctx_p, ref **r)
{
    int code=0, i, components = 0;
    ref *space, *tempref, valref, ICCdict, sref;

    space = *r;
    if (!r_is_array(space))
        return_error(gs_error_typecheck);
     
    if (r_size(space) != 2)
        return_error(gs_error_rangecheck);

    code = array_get(imemory, space, 1, &ICCdict);
    if (code < 0)
        return code;

    check_type(ICCdict, t_dictionary);

    code = dict_find_string(&ICCdict, "N", &tempref);
    if (code < 0)
        return code;
    if (code == 0)
        return gs_note_error(gs_error_undefined);
    if (!r_has_type(tempref, t_null)) {
        if (!r_has_type(tempref, t_integer))
            return_error(gs_error_typecheck);
        components = tempref->value.intval;
    } else
        return_error(gs_error_typecheck);
    code = dict_find_string(&ICCdict, "DataSource", &tempref);
    if (code <= 0)
        return_error(gs_error_typecheck);
    if (!r_has_type(tempref, t_null)) {
        if (!r_has_type(tempref, t_string) && !r_has_type(tempref, t_file))
            return_error(gs_error_typecheck);
    } else
        return_error(gs_error_typecheck);

     
    code = dict_find_string(&ICCdict, "Range", &tempref);
    if (code > 0 && !r_has_type(tempref, t_null)) {
        if (!r_is_array(tempref))
            return_error(gs_error_typecheck);
        if (r_size(tempref) < (components * 2))
            return_error(gs_error_rangecheck);
        for (i=0;i<components * 2;i++) {
            code = array_get(imemory, tempref, i, &valref);
            if (code < 0)
                return code;
            if (!r_has_type(&valref, t_integer) && !r_has_type(&valref, t_real))
                return_error(gs_error_typecheck);
        }
    }
    code = dict_find_string(&ICCdict, "Alternate", &tempref);
    if (code > 0 && !r_has_type(tempref, t_null)) {
        ref_assign(*r, tempref);
        if (r_has_type(tempref, t_name)) {
            name_string_ref(imemory, tempref, &sref);
            if (sref.value.bytes && strncmp((const char *)sref.value.bytes, "Pattern", 7) == 0)
                return_error(gs_error_typecheck);
        } else {
            if (r_is_array(tempref)) {
                code = array_get(imemory, tempref, 0, &valref);
                if (code < 0)
                    return code;
                if (!r_has_type(&valref, t_name) && !r_has_type(&valref, t_string))
                    return_error(gs_error_typecheck);
                if (r_has_type(&valref, t_name))
                    name_string_ref(imemory, &valref, &sref);
                else
                    sref.value.bytes = valref.value.bytes;
                if (sref.value.bytes && strncmp((const char *)sref.value.bytes, "Pattern", 7) == 0)
                    return_error(gs_error_typecheck);
            } else
                return_error(gs_error_typecheck);
        }
    } else {
        ref nameref;

        switch (components) {
            case 1:
                code = name_enter_string(imemory, "DeviceGray", &nameref);
                break;
            case 3:
                code = name_enter_string(imemory, "DeviceRGB", &nameref);
                break;
            case 4:
                code = name_enter_string(imemory, "DeviceCMYK", &nameref);
                break;
            default:
                return_error(gs_error_rangecheck);
        }
        if (code < 0)
            return code;
         
        code = idict_put_string(&ICCdict, "Alternate", &nameref);
        if (code < 0)
            return code;

         
        return validateiccspace(i_ctx_p, r);
    }
    return code;
}
