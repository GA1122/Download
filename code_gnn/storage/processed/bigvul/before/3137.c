static int setciedefgspace(i_ctx_t * i_ctx_p, ref *r, int *stage, int *cont, int CIESubst)
{
    int code = 0;
    ref CIEDict, *nocie;
    ulong dictkey;
    gs_md5_state_t md5;
    byte key[16];

    if (i_ctx_p->language_level < 3)
        return_error(gs_error_undefined);

    code = dict_find_string(systemdict, "NOCIE", &nocie);
    if (code > 0) {
        if (!r_has_type(nocie, t_boolean))
            return_error(gs_error_typecheck);
        if (nocie->value.boolval)
            return setcmykspace(i_ctx_p, r, stage, cont, 1);
    }

    *cont = 0;
    code = array_get(imemory, r, 1, &CIEDict);
    if (code < 0)
        return code;
    if ((*stage) > 0) {
        gs_client_color cc;
        int i;

        cc.pattern = 0x00;
        for (i=0;i<4;i++)
            cc.paint.values[i] = 0;
        code = gs_setcolor(igs, &cc);
        *stage = 0;
        return code;
    }
    gs_md5_init(&md5);
     
    dictkey = 0;
    if (hashciedefgspace(i_ctx_p, r, &md5)) {
         
        gs_md5_finish(&md5, key);
        dictkey = *(ulong *)&key[sizeof(key) - sizeof(ulong)];
    } else {
        gs_md5_finish(&md5, key);
    }
    code = ciedefgspace(i_ctx_p, &CIEDict,dictkey);
    *cont = 1;
    (*stage)++;
    return code;
}
