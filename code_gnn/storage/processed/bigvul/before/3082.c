static int graybasecolor(i_ctx_t * i_ctx_p, ref *space, int base, int *stage, int *cont, int *stack_depth)
{
    os_ptr op = osp;
    float Gray, RGB[3];

    *cont = 0;
    *stage = 0;
    check_op(1);
    if (!r_has_type(op, t_integer)) {
        if (r_has_type(op, t_real)) {
            Gray = op->value.realval;
        } else
            return_error(gs_error_typecheck);
    } else
        Gray = (float)op->value.intval;

    if (Gray < 0 || Gray > 1)
        return_error(gs_error_rangecheck);

    switch (base) {
        case 0:
             
            make_real(op, Gray);
            break;
        case 1:
             
        case 2:
             
            push(2);
            RGB[0] = RGB[1] = RGB[2] = Gray;
            if (base == 1)
                 
                rgb2hsb((float *)&RGB);
            make_real(&op[-2], RGB[0]);
            make_real(&op[-1], RGB[1]);
            make_real(op, RGB[2]);
            break;
        case 3:
             
            push(3);
            make_real(&op[-3], (float)0);
            make_real(&op[-2], (float)0);
            make_real(&op[-1], (float)0);
            make_real(op, (float)1.0 - Gray);
            break;
        default:
            return_error(gs_error_undefined);
    }
    return 0;
}
