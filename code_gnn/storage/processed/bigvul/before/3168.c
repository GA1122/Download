static int validatelabspace(i_ctx_t * i_ctx_p, ref **r)
{
    int code=0;
    ref *space, labdict;

    space = *r;
    if (!r_is_array(space))
        return_error(gs_error_typecheck);
     
    if (r_size(space) < 2)
        return_error(gs_error_rangecheck);
    code = array_get(imemory, space, 1, &labdict);
    if (code < 0)
        return code;
    check_type(labdict, t_dictionary);
     
    code = checkWhitePoint(i_ctx_p, &labdict);
    if (code != 0)
        return code;
     
    code = checkBlackPoint(i_ctx_p, &labdict);
    if (code < 0)
        return code;
     
    code = checkrangeab(i_ctx_p, &labdict);
    if (code < 0)
        return code;
    *r = 0;   
    return 0;
}
