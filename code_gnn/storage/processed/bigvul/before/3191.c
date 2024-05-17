zsetstrokecolorspace(i_ctx_t * i_ctx_p)
{
    int code;

    code = zswapcolors(i_ctx_p);
    if (code < 0)
        return code;

     
     
    check_estack(1);
     
    push_op_estack(setstrokecolorspace_cont);

    code = zsetcolorspace(i_ctx_p);
    if (code >= 0)
        return o_push_estack;

    return code;
}
