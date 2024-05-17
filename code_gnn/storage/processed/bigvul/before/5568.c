PreserveTrMode(xps_context_t *ctx)
{

    gs_c_param_list list;
    dev_param_req_t request;
    gs_param_name ParamName = "PreserveTrMode";
    gs_param_typed_value Param;
    char *data;
    gs_gstate *pgs = ctx->pgs;
    int code = 0;

     
    data = (char *)gs_alloc_bytes(ctx->memory, 15, "temporary special_op string");
    memset(data, 0x00, 15);
    memcpy(data, "PreserveTrMode", 15);
    gs_c_param_list_write(&list, ctx->memory);
     
    Param.type = gs_param_type_null;
    list.procs->xmit_typed((gs_param_list *)&list, ParamName, &Param);
     
    request.Param = data;
    request.list = &list;

    code = dev_proc(gs_currentdevice(pgs), dev_spec_op)(gs_currentdevice(pgs), gxdso_get_dev_param,
                                                        &request, sizeof(dev_param_req_t));

    if (code != gs_error_undefined) {
         
        gs_c_param_list_read(&list);
        list.procs->xmit_typed((gs_param_list *)&list, ParamName, &Param);

        if (Param.type != gs_param_type_bool) {
             
            gs_free_object(ctx->memory, data,"temporary special_op string");
            gs_c_param_list_release(&list);
            return gs_error_typecheck;
        }

        if (Param.value.b) {
            code = 1;
        } else {
            code = 0;
        }
    } else {
        code = 0;
    }
    gs_free_object(ctx->memory, data,"temporary special_op string");
    gs_c_param_list_release(&list);
    return code;
}
