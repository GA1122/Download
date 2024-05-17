static int deviceninitialproc(i_ctx_t *i_ctx_p, ref *space)
{
    gs_client_color cc;
    int i, num_components, code;
    ref namesarray;

    code = array_get(imemory, space, 1, &namesarray);
    if (code < 0)
        return code;
    num_components = r_size(&namesarray);
    cc.pattern = 0x00;
    for (i=0;i<num_components;i++)
        cc.paint.values[i] = 1.0;
    return gs_setcolor(igs, &cc);
}
