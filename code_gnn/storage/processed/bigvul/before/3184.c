zsetcolorspace(i_ctx_t * i_ctx_p)
{
    os_ptr  op = osp;
    es_ptr ep;
    int code, depth;
    bool is_CIE;

     
    check_op(1);
     
    if (!r_has_type(op, t_name))
        if (!r_is_array(op))
            return_error(gs_error_typecheck);

    code = validate_spaces(i_ctx_p, op, &depth);
    if (code < 0)
        return code;

    is_CIE = istate->use_cie_color.value.boolval;

     
    if (is_same_colorspace(i_ctx_p, op, &istate->colorspace[0].array, is_CIE)) {
        PS_colour_space_t *cspace;

         
        code = get_space_object(i_ctx_p, &istate->colorspace[0].array, &cspace);
        if (code < 0)
            return 0;
        if (cspace->initialcolorproc) {
            cspace->initialcolorproc(i_ctx_p, &istate->colorspace[0].array);
        }
         
        pop(1);
        return 0;
    }
     
     
    check_estack(5);
     
    ep = esp += 1;
    make_int(ep, 0);
     
    ep = esp += 1;
    make_int(ep, depth);
     
    ep = esp += 1;
    make_int(ep, 0);
     
    ep = esp += 1;
    *ep = *op;
     
    push_op_estack(setcolorspace_cont);
    return o_push_estack;
}
