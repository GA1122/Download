zgetuseciecolor(i_ctx_t * i_ctx_p)
{
    os_ptr  op = osp;

    push(1);
    *op = istate->use_cie_color;
    return 0;
}
