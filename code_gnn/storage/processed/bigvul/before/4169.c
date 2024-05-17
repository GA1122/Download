ztoken(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;

    switch (r_type(op)) {
        default:
            return_op_typecheck(op);
        case t_file: {
            stream *s;
            scanner_state state;

            check_read_file(i_ctx_p, s, op);
            check_ostack(1);
            gs_scanner_init(&state, op);
            return token_continue(i_ctx_p, &state, true);
        }
        case t_string: {
            ref token;
             
            int orig_ostack_depth = ref_stack_count(&o_stack) - 1;
            int code;

             
            if (!r_has_attr(op, a_read))
                return_error(gs_error_invalidaccess);
            code = gs_scan_string_token(i_ctx_p, op, &token);
            switch (code) {
            case scan_EOF:       
                make_false(op);
                return 0;
            default:
                if (code < 0) {
                     
                    if (orig_ostack_depth < ref_stack_count(&o_stack))
                        pop(ref_stack_count(&o_stack)- orig_ostack_depth);
                    return code;
                }
            }
            push(2);
            op[-1] = token;
            make_true(op);
            return 0;
        }
    }
}
