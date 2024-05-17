zsethsbcolor(i_ctx_t * i_ctx_p)
{
    os_ptr  op = osp;    
    int code, i;
    float values[3];

     
    code = float_params(op, 3, (float *)&values);
    if (code < 0)
        return code;
     
    for (i = 0;i < 3; i++) {
        if (values[i] < 0)
            values[i] = 0;
        else if (values[i] > 1)
            values[i] = 1;
    }

    hsb2rgb((float *)&values);

    code = make_floats(&op[-2], (const float *)&values, 3);
    if (code < 0)
        return code;

     
     
    check_estack(5);
    push_mark_estack(es_other, colour_cleanup);
    esp++;
     
    make_int(esp, 1);
    esp++;
     
    make_int(esp, 0);
     
    push_op_estack(setdevicecolor_cont);
    return o_push_estack;
}
