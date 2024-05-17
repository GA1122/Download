zsetpagedevice(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    int code;

 
    if (r_has_type(op, t_dictionary)) {
        check_dict_read(*op);
#if 0	 
         
        if (!(r_is_local(op)))
            return_error(gs_error_invalidaccess);
#endif	 
         
        code = zreadonly(i_ctx_p);
        if (code < 0)
            return code;
    } else {
        check_type(*op, t_null);
    }
    istate->pagedevice = *op;
    pop(1);
    return 0;
}
