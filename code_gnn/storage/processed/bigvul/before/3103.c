static int indexedbasecolor(i_ctx_t * i_ctx_p, ref *space, int base, int *stage, int *cont, int *stack_depth)
{
    int code;

    if (*stage == 0) {
         
        gs_color_space *pcs;
        pcs = gs_currentcolorspace(igs);

         
        *stage = 1;
        *cont = 1;

         
        if (pcs->params.indexed.use_proc) {
            es_ptr ep = ++esp;
            ref proc;

             
            check_estack(1);
            code = array_get(imemory, space, 3, &proc);
            if (code < 0)
                return code;
            *ep = proc;	 
            return o_push_estack;
        } else {
            int i, index;
            os_ptr op = osp;
            unsigned char *ptr = (unsigned char *)pcs->params.indexed.lookup.table.data;

            *stage = 0;
             
             
            if (!r_has_type(op, t_integer))
                return_error (gs_error_typecheck);
            index = op->value.intval;
             
            pop(1);
            op = osp;

             
            push(pcs->params.indexed.n_comps);
            op -= pcs->params.indexed.n_comps - 1;

             
            ptr += index * pcs->params.indexed.n_comps;

             
            for (i = 0; i < pcs->params.indexed.n_comps; i++, op++) {
                float rval = (*ptr++) / 255.0;
                make_real(op, rval);
            }
            return 0;
        }
    } else {
        *stage = 0;
        *cont = 1;
        return 0;
    }
}
