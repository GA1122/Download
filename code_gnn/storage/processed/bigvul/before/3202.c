zcvx(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    ref *aop;
    uint opidx;

    check_op(1);
     
    if (r_has_type(op, t_operator) &&
        ((opidx = op_index(op)) == 0 ||
         op_def_is_internal(op_index_def(opidx)))
        )
        return_error(gs_error_rangecheck);
    aop = ACCESS_REF(op);
    r_set_attrs(aop, a_executable);
    return 0;
}
