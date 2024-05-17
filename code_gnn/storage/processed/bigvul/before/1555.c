zmakewordimagedevice(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    os_ptr op1 = op - 1;
    gs_matrix imat;
    gx_device *new_dev;
    const byte *colors;
    int colors_size;
    int code;

    check_int_leu(op[-3], max_uint >> 1);	 
    check_int_leu(op[-2], max_uint >> 1);	 
    check_type(*op, t_boolean);
    if (r_has_type(op1, t_null)) {	 
        colors = 0;
        colors_size = -24;	 
    } else if (r_has_type(op1, t_integer)) {
         
        if (op1->value.intval != 16 && op1->value.intval != 24 &&
            op1->value.intval != 32
            )
            return_error(gs_error_rangecheck);
        colors = 0;
        colors_size = -op1->value.intval;
    } else {
        check_type(*op1, t_string);	 
        if (r_size(op1) > 3 * 256)
            return_error(gs_error_rangecheck);
        colors = op1->value.bytes;
        colors_size = r_size(op1);
    }
    if ((code = read_matrix(imemory, op - 4, &imat)) < 0)
        return code;
     
    code = gs_makewordimagedevice(&new_dev, &imat,
                                  (int)op[-3].value.intval,
                                  (int)op[-2].value.intval,
                                  colors, colors_size,
                                  op->value.boolval, true, imemory);
    if (code == 0) {
        new_dev->memory = imemory;
        make_tav(op - 4, t_device, imemory_space(iimemory) | a_all,
                 pdevice, new_dev);
        pop(4);
    }
    return code;
}
