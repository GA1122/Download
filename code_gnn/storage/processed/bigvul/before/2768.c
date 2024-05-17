errorexec_find(i_ctx_t *i_ctx_p, ref *perror_object)
{
    long i;
    const ref *ep;

    for (i = 0; (ep = ref_stack_index(&e_stack, i)) != 0; ++i) {
        if (r_is_estack_mark(ep)) {
            if (ep->value.opproc == oparray_cleanup) {
                 
                uint opindex = (uint)ep[1].value.intval;
                if (opindex == 0)  
                    continue;
                op_index_ref(imemory, opindex, perror_object);
                return 1;
            }
            if (ep->value.opproc == oparray_no_cleanup)
                return 0;        
            if (ep->value.opproc == errorexec_cleanup) {
                if (r_has_type(ep + 1, t_null))
                    return 0;
                *perror_object = ep[1];  
                return 1;
            }
        }
    }
    return 0;
}
