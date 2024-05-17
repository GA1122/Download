znoaccess(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;

    check_op(1);
    if (r_has_type(op, t_dictionary)) {
        ref *aop = dict_access_ref(op);

         
        if (!r_has_attrs(aop, a_write)) {
            if (!r_has_attrs(aop, a_read) && !r_has_attrs(aop, a_execute)) {
                 
                return 0;
            }
            return_error(gs_error_invalidaccess);
        }

         
        if (dict_is_permanent_on_dstack(op))
            return_error(gs_error_invalidaccess);
    }
    return access_check(i_ctx_p, 0, true);
}
