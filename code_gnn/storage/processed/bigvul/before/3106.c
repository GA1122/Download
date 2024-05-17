static int indexedvalidate(i_ctx_t *i_ctx_p, ref *space, float *values, int num_comps)
{
    int code, integer;
    float fraction;
    ref hival;
    os_ptr op = osp;

    if (num_comps < 1)
        return_error(gs_error_stackunderflow);

    if (!r_has_type(op, t_integer) && !r_has_type(op, t_real))
        return_error(gs_error_typecheck);

    code = array_get(imemory, space, 2, &hival);
    if (code < 0)
        return code;

    if (*values > hival.value.intval)
        *values = (float)hival.value.intval;

    if (*values < 0)
        *values = 0;

     
    integer = (int)floor(*values);
    fraction = *values - integer;

    if (fraction >= 0.5)
        *values = (float)(integer + 1);
    else
        *values = (float)integer;

    return 0;
}
