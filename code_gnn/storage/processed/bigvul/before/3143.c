setdevicecolor_cont(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    es_ptr ep = esp, pstage;
    int code = 0, stage, base;

    pstage = ep;
    base = (int)ep[-1].value.intval;
    stage = (int)pstage->value.intval;
     
    check_estack(1);
     
    check_ostack(1);
     
    push_op_estack(setdevicecolor_cont);

    do {
        switch(stage) {
            case 0:
                make_int(pstage, ++stage);
                push(1);
                switch(base) {
                    case 0:  
                        code = name_enter_string(imemory, "DeviceGray", op);
                        break;
                    case 1:  
                        code = name_enter_string(imemory, "DeviceRGB", op);
                        break;
                    case 2:  
                        code = name_enter_string(imemory, "DeviceCMYK", op);
                        break;
                }
                if (code < 0)
                    return code;
                code = zsetcolorspace(i_ctx_p);
                if (code != 0)
                    return code;
                break;
            case 1:
                make_int(pstage, ++stage);
                code = zsetcolor(i_ctx_p);
                if (code != 0)
                    return code;
                break;
            case 2:
                esp -= 3;
                return o_pop_estack;
                break;
        }
    }while(1);
    return 0;
}
