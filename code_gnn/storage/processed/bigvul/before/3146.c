static int setgrayspace(i_ctx_t * i_ctx_p, ref *r, int *stage, int *cont, int CIESubst)
{
    os_ptr op = osp;
    gs_color_space  *pcs;
    int code=0;
    ref stref;

    do {
        switch (*stage) {
            case 0:
                if (istate->use_cie_color.value.boolval && !CIESubst) {
                    byte *body;
                    ref *nosubst;

                    code = dict_find_string(systemdict, "NOSUBSTDEVICECOLORS", &nosubst);
                    if (code != 0) {
                        if (!r_has_type(nosubst, t_boolean))
                            return_error(gs_error_typecheck);
                    }
                    if (code != 0 && nosubst->value.boolval) {
                        *stage = 4;
                        *cont = 1;
                        body = ialloc_string(32, "string");
                        if (body == 0)
                            return_error(gs_error_VMerror);
                        memcpy(body, "/DefaultGray ..nosubstdevicetest",32);
                        make_string(&stref, a_all | icurrent_space, 32, body);
                        r_set_attrs(&stref, a_executable);
                        esp++;
                        ref_assign(esp, &stref);
                        return o_push_estack;
                    } else {
                        *stage = 2;
                        *cont = 1;
                        body = ialloc_string(47, "string");
                        if (body == 0)
                            return_error(gs_error_VMerror);
                        memcpy(body, "{/DefaultGray /ColorSpace findresource} stopped",47);
                        make_string(&stref, a_all | icurrent_space, 47, body);
                        r_set_attrs(&stref, a_executable);
                        esp++;
                        ref_assign(esp, &stref);
                        return o_push_estack;
                    }
                    break;
                }
                 
            case 1:
                pcs = gs_cspace_new_DeviceGray(imemory);
                if (pcs == NULL)
                    return_error(gs_error_VMerror);
                code = gs_setcolorspace(igs, pcs);
                if (code >= 0) {
                    gs_client_color *pcc = gs_currentcolor_inline(igs);

                    cs_adjust_color_count(igs, -1);  
                    pcc->paint.values[0] = (0);
                    pcc->pattern = 0;		 
                    gx_unset_dev_color(igs);
                }
                rc_decrement_only_cs(pcs, "zsetdevcspace");
                *cont = 0;
                *stage = 0;
                break;
            case 2:
                if (!r_has_type(op, t_boolean))
                    return_error(gs_error_typecheck);
                if (op->value.boolval) {
                     
                    pop(1);
                    *stage = 1;
                    break;
                }
                pop(1);
                *cont = 1;
                *stage = 3;
                code = setcolorspace_nosubst(i_ctx_p);
                if (code != 0)
                    return code;
                break;
            case 3:
                 
                *cont = 0;
                *stage = 0;
                break;
            case 4:
                 
                if (!r_has_type(op, t_boolean))
                    return_error(gs_error_typecheck);
                pop(1);
                *stage = 1;
                *cont = 1;
                if (op->value.boolval) {
                    *stage = 5;
                    code = setcolorspace_nosubst(i_ctx_p);
                    if (code != 0)
                        return code;
                }
                break;
            case 5:
                 
                *stage = 1;
                *cont = 1;
                code = zincludecolorspace(i_ctx_p);
                if (code != 0)
                    return code;
                break;
        }
    } while (*stage);
    return code;
}