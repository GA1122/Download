zcurrenthsbcolor(i_ctx_t * i_ctx_p)
{
    int code, depth;

    code = validate_spaces(i_ctx_p, &istate->colorspace[0].array, &depth);
    if (code < 0)
        return code;

    code = zcurrentcolor(i_ctx_p);
    if (code < 0)
        return code;
     
     
    check_estack(7);
    push_mark_estack(es_other, colour_cleanup);
    esp++;
     
    make_int(&esp[0], 0);
    esp++;
     
    make_int(&esp[0], 1);
    make_int(&esp[1], 1);
     
    make_int(&esp[2], 0);
     
    esp[3] = istate->colorspace[0].array;
    esp += 3;  
     
    push_op_estack(currentbasecolor_cont);
    return o_push_estack;
}
