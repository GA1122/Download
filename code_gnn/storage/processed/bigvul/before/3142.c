setcolorspace_nosubst(i_ctx_t * i_ctx_p)
{
    os_ptr  op = osp;
    es_ptr ep;
    int code, depth;

     
    check_op(1);
     
    if (!r_has_type(op, t_name))
        if (!r_is_array(op))
            return_error(gs_error_typecheck);

    code = validate_spaces(i_ctx_p, op, &depth);
    if (code < 0)
        return code;

     
     
    check_estack(5);
     
    ep = esp += 1;
    make_int(ep, 1);
     
    ep = esp += 1;
    make_int(ep, depth);
     
    ep = esp += 1;
    make_int(ep, 0);
     
    ep = esp += 1;
    *ep = *op;
     
    push_op_estack(setcolorspace_cont);
    return o_push_estack;
}
