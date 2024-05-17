zgetdevice(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    const gx_device *dev;

    check_type(*op, t_integer);
    if (op->value.intval != (int)(op->value.intval))
        return_error(gs_error_rangecheck);	 
    dev = gs_getdevice((int)(op->value.intval));
    if (dev == 0)		 
        return_error(gs_error_rangecheck);
     
     
    make_tav(op, t_device, avm_foreign | a_readonly, pdevice,
             (gx_device *) dev);
    return 0;
}
