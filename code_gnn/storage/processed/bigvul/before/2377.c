cond_continue(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    es_ptr ep = esp;
    int code;

     
     
     
     
    check_type(*op, t_boolean);
    if (op->value.boolval) {	 
        array_get(imemory, ep, 1L, ep);
        esfile_check_cache();
        code = o_pop_estack;
    } else if (r_size(ep) > 2) {	 
        const ref_packed *elts = ep->value.packed;

        check_estack(2);
        r_dec_size(ep, 2);
        elts = packed_next(elts);
        elts = packed_next(elts);
        ep->value.packed = elts;
        array_get(imemory, ep, 0L, ep + 2);
        make_op_estack(ep + 1, cond_continue);
        esp = ep + 2;
        esfile_check_cache();
        code = o_push_estack;
    } else {			 
        esp = ep - 1;
        code = o_pop_estack;
    }
    pop(1);			 
    return code;
}
