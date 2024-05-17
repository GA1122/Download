zspec_op(i_ctx_t *i_ctx_p)
{
    os_ptr  op = osp;
    gx_device *dev = gs_currentdevice(igs);
    int i, nprocs = sizeof(spec_op_defs) / sizeof(spec_op_t), code, proc = -1;
    ref opname, nref, namestr;
    char *data;

     
    check_op(1);
    if (!r_has_type(op, t_name))
        return_error(gs_error_typecheck);

    ref_assign(&opname, op);

     
    for (i=0;i<nprocs;i++) {
        code = names_ref(imemory->gs_lib_ctx->gs_name_table, (const byte *)spec_op_defs[i].name, strlen(spec_op_defs[i].name), &nref, 0);
        if (code < 0)
            return code;
        if (name_eq(&opname, &nref)) {
            proc = i;
            break;
        }
    }

    if (proc < 0)
        return_error(gs_error_undefined);

    pop(1);      
    op = osp;

    switch(proc) {
        case 0:
            {
                stack_param_list list;
                dev_param_req_t request;
                ref rkeys;
                 
                check_op(1);
                if (!r_has_type(op, t_name))
                    return_error(gs_error_typecheck);

                ref_assign(&opname, op);
                name_string_ref(imemory, &opname, &namestr);

                data = (char *)gs_alloc_bytes(imemory, r_size(&namestr) + 1, "temporary special_op string");
                if (data == 0)
                    return_error(gs_error_VMerror);
                memset(data, 0x00, r_size(&namestr) + 1);
                memcpy(data, namestr.value.bytes, r_size(&namestr));

                 
                pop (1);
                 
                make_null(&rkeys);
                stack_param_list_write(&list, &o_stack, &rkeys, iimemory);
                 
                request.Param = data;
                request.list = &list;

                code = dev_proc(dev, dev_spec_op)(dev, gxdso_get_dev_param, &request, sizeof(dev_param_req_t));

                gs_free_object(imemory, data, "temporary special_op string");

                if (code < 0) {
                    if (code == gs_error_undefined) {
                        op = osp;
                        push(1);
                        make_bool(op, 0);
                    } else
                        return_error(code);
                } else {
                    op = osp;
                    push(1);
                    make_bool(op, 1);
                }
            }
            break;
        default:
             
            return_error(gs_error_undefined);
            break;
    }
    return 0;
}
