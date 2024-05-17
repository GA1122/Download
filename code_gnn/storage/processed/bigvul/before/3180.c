zcurrenttransfer(i_ctx_t *i_ctx_p)
{
    os_ptr  op = osp;

    push(1);
    *op = istate->transfer_procs.gray;
    return 0;
}
