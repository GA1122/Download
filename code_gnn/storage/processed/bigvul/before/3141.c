setcolorspace_cont(i_ctx_t *i_ctx_p)
{
    ref arr, *parr = &arr;
    os_ptr op = osp;
    es_ptr ep = esp, pdepth, pstage, pCIESubst;
    int i, code = 0, stage, cont, CIESubst = 0;
    unsigned int depth;
    PS_colour_space_t *obj;

    pCIESubst = &ep[-3];
    pdepth = &ep[-2];
    pstage = &ep[-1];

    CIESubst = (int)pCIESubst->value.intval;
    depth = (unsigned int)pdepth->value.intval;
    stage = (int)pstage->value.intval;
     
    check_estack(1);
    push_op_estack(setcolorspace_cont);

    while (code == 0 && depth) {
        ref_assign(&arr, ep);
         
        for (i = 0;i < depth;i++) {
            code = get_space_object(i_ctx_p, parr, &obj);
            if (code < 0)
                return code;

            if (i < (depth - 1)) {
                if (!obj->alternateproc) {
                    return_error(gs_error_typecheck);
                }
                code = obj->alternateproc(i_ctx_p, parr, &parr, &CIESubst);
                if (code < 0)
                    return code;
            }
        }

        code = obj->setproc(i_ctx_p, parr, &stage, &cont, CIESubst);
        make_int(pstage, stage);
        if (code != 0) {
            if (code < 0 && code != gs_error_stackoverflow)
                esp -= 5;
            return code;
        }
        if (!cont) {
             
            make_int(pdepth, --depth);
            parr = &arr;
        }
    }
    if (code == 0) {
         
        esp -= 5;
        op = osp;
        istate->colorspace[0].array = *op;
         
        pop(1);
        code = o_pop_estack;
    }
    return code;
}
