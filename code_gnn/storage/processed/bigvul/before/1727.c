zcurrentcolorspace(i_ctx_t * i_ctx_p)
{
    os_ptr  op = osp;    
    int code;
    ref namestr,stref;
    byte *body;

     
    if (r_has_type(&istate->colorspace[0].array, t_name)) {
        name_string_ref(imemory, &istate->colorspace[0].array, &namestr);
        if (r_size(&namestr) == 10 && !memcmp(namestr.value.bytes, "DeviceGray", 10)) {
            body = ialloc_string(32, "string");
            if (body == 0)
                return_error(gs_error_VMerror);
            memcpy(body, "systemdict /DeviceGray_array get", 32);
            make_string(&stref, a_all | icurrent_space, 32, body);
        } else {
            if (r_size(&namestr) == 9 && !memcmp(namestr.value.bytes, "DeviceRGB", 9)) {
                body = ialloc_string(31, "string");
                if (body == 0)
                    return_error(gs_error_VMerror);
                memcpy(body, "systemdict /DeviceRGB_array get", 31);
                make_string(&stref, a_all | icurrent_space, 31, body);
            } else {
                if (r_size(&namestr) == 10 && !memcmp(namestr.value.bytes, "DeviceCMYK", 10)) {
                    body = ialloc_string(32, "string");
                    if (body == 0)
                        return_error(gs_error_VMerror);
                    memcpy(body, "systemdict /DeviceCMYK_array get", 32);
                    make_string(&stref, a_all | icurrent_space, 32, body);
                } else {
                     
                    push(1);
                    code = ialloc_ref_array(op, a_all, 1, "currentcolorspace");
                    if (code < 0)
                        return code;
                    refset_null(op->value.refs, 1);
                    ref_assign_old(op, op->value.refs,
                                   &istate->colorspace[0].array,
                                   "currentcolorspace");
                    return 0;
                }
            }
        }
        r_set_attrs(&stref, a_executable);
        esp++;
        ref_assign(esp, &stref);
        return o_push_estack;
    } else {
         
        push(1);
        *op = istate->colorspace[0].array;
    }
    return 0;
}
