static int setcalrgbspace(i_ctx_t * i_ctx_p, ref *r, int *stage, int *cont, int CIESubst)
{
    ref rgbdict;
    int code = 0;
    float                   gamma[3], white[3], black[3], matrix[9];
    static const float      dflt_gamma[3] = { 1.0, 1.0, 1.0 };
    static const float      dflt_black[3] = {0,0,0}, dflt_white[3] = {0,0,0};
    static const float      dflt_matrix[9] = {1,0,0,0,1,0,0,0,1};
    int i;
    gs_client_color cc;

    *cont = 0;
    code = array_get(imemory, r, 1, &rgbdict);
    if (code < 0)
        return code;
 
    code = dict_floats_param( imemory,
                              &rgbdict,
                              "Gamma",
                              3,
                              gamma,
                              dflt_gamma );
    if (code < 0)
        return code;
    if (gamma[0] <= 0 || gamma[1] <= 0 || gamma[2] <= 0)
        return_error(gs_error_rangecheck);
    code = dict_floats_param( imemory,
                              &rgbdict,
                              "BlackPoint",
                              3,
                              black,
                              dflt_black );
    if (code < 0)
        return code;
    code = dict_floats_param( imemory,
                              &rgbdict,
                              "WhitePoint",
                              3,
                              white,
                              dflt_white );
    if (code < 0)
        return code;
    if (white[0] <= 0 || white[1] != 1.0 || white[2] <= 0)
        return_error(gs_error_rangecheck);
    code = dict_floats_param( imemory,
                              &rgbdict,
                              "Matrix",
                              9,
                              matrix,
                              dflt_matrix );
    if (code < 0)
        return code;
    code = seticc_cal(i_ctx_p, white, black, gamma, matrix, 3, rgbdict.value.saveid);
    if ( code < 0)
        return gs_rethrow(code, "setting CalRGB  color space");
    cc.pattern = 0x00;
    for (i=0;i<3;i++)
        cc.paint.values[i] = 0;
    code = gs_setcolor(igs, &cc);
    return code;
}
