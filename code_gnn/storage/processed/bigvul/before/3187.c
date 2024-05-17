zsetgray(i_ctx_t * i_ctx_p)
{
    os_ptr  op = osp;    
    float value;
    int code;

     
    code = float_params(op, 1, &value);
    if (code < 0)
        return code;
     
    if (value < 0)
        value = 0;
    else if (value > 1)
        value = 1;
    code = make_floats(op, &value, 1);
    if (code < 0)
        return code;

     
     
    check_estack(5);
    push_mark_estack(es_other, colour_cleanup);
    esp++;
     
    make_int(esp, 0);
    esp++;
     
    make_int(esp, 0);
     
    push_op_estack(setdevicecolor_cont);
    return o_push_estack;
}
