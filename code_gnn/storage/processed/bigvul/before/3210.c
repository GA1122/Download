zsetcolor(i_ctx_t * i_ctx_p)
{
    os_ptr                  op = osp;
    es_ptr ep;
    const gs_color_space *  pcs = gs_currentcolorspace(igs);
    gs_client_color         cc;
    int                     n_comps, n_numeric_comps, num_offset = 0, code, depth;
    PS_colour_space_t *space;

     
    cc.pattern = 0;

     
    if ((n_comps = cs_num_components(pcs)) < 0) {
        n_comps = -n_comps;
        if (r_has_type(op, t_dictionary)) {
            ref     *pImpl, pPatInst;

            if ((code = dict_find_string(op, "Implementation", &pImpl)) < 0)
                return code;
            if (code > 0) {
                code = array_get(imemory, pImpl, 0, &pPatInst);
                if (code < 0)
                    return code;
                cc.pattern = r_ptr(&pPatInst, gs_pattern_instance_t);
                n_numeric_comps = ( pattern_instance_uses_base_space(cc.pattern)
                      ? n_comps - 1
                      : 0 );
            } else
                n_numeric_comps = 0;
        } else
            n_numeric_comps = 0;
        num_offset = 1;
    } else
        n_numeric_comps = n_comps;

     
    code = float_params(op - num_offset, n_numeric_comps, cc.paint.values);
    if (code < 0)
        return code;
     
     
     
    memset(cc.paint.values + n_numeric_comps, 0,
            sizeof(cc.paint.values) - sizeof(*cc.paint.values)*n_numeric_comps);

    code = get_space_object(i_ctx_p, &istate->colorspace[0].array, &space);
    if (code < 0)
        return code;
    if (space->validatecomponents) {
        code = space->validatecomponents(i_ctx_p,
                                         &istate->colorspace[0].array,
                                         cc.paint.values, n_numeric_comps);
        if (code < 0)
            return code;
    }

     
    if ((code = gs_setcolor(igs, &cc)) >= 0) {

        if (n_comps > n_numeric_comps) {
            istate->pattern[0] = *op;       
        }
    }

     
    code = validate_spaces(i_ctx_p, &istate->colorspace[0].array, &depth);
    if (code < 0)
        return code;
     
     
    check_estack(5);
     
    ep = esp += 1;
    make_int(ep, 0);
     
    ep = esp += 1;
    make_int(ep, 0);
     
    ep = esp += 1;
    make_int(ep, 0);
     
    ep = esp += 1;
    *ep = istate->colorspace[0].array;
     
    push_op_estack(setcolor_cont);
    return o_push_estack;
}
