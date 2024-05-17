setcolor_cont(i_ctx_t *i_ctx_p)
{
    ref arr, *parr = &arr;
    es_ptr ep = esp;
    int i=0, code = 0, usealternate, stage, stack_depth, CIESubst = 0, IsICC = 0;
    unsigned int depth;
    PS_colour_space_t *obj;

    stack_depth = (int)ep[-3].value.intval;
    depth = (unsigned int)ep[-2].value.intval;
    stage = (int)ep[-1].value.intval;
     
    check_estack(1);
    push_op_estack(setcolor_cont);

    while (code == 0) {
        ref_assign(&arr, ep);
         
        for (i=0;i<=depth;i++) {
            code = get_space_object(i_ctx_p, parr, &obj);
            if (code < 0)
                return code;

            if (strcmp(obj->name, "ICCBased") == 0)
                IsICC = 1;

            if (i < (depth)) {
                if (!obj->alternateproc) {
                    return_error(gs_error_typecheck);
                }
                code = obj->alternateproc(i_ctx_p, parr, &parr, &CIESubst);
                if (code < 0)
                    return code;
            }
        }
        if (obj->runtransformproc) {
            code = obj->runtransformproc(i_ctx_p, &istate->colorspace[0].array, &usealternate, &stage, &stack_depth);
            make_int(&ep[-3], stack_depth);
            make_int(&ep[-1], stage);
            if (code != 0) {
                return code;
            }
            make_int(&ep[-2], ++depth);
            if (!usealternate)
                break;
        } else
            break;
    }
     
    if (IsICC && depth == 0) {
        code = gx_set_dev_color(i_ctx_p->pgs);
        if (code < 0)
            return code;
    }

     
    obj->numcomponents(i_ctx_p, parr, &i);
    pop(i);
    esp -= 5;
    return o_pop_estack;
}
