static int setseparationspace(i_ctx_t * i_ctx_p, ref *sepspace, int *stage, int *cont, int CIESubst)
{
    os_ptr op = osp;    
    int code = 0;
    ref sname, proc;
    ref name_none, name_all;
    separation_type sep_type;
    ref_colorspace cspace_old;
    gs_color_space *pcs;
    gs_color_space * pacs;
    gs_function_t *pfn = NULL;
    gs_client_color cc;

    if (i_ctx_p->language_level < 2)
        return_error(gs_error_undefined);

    *cont = 0;
    if ((*stage) == 0) {
        code = array_get(imemory, sepspace, 3, &proc);
        if (code < 0)
            return code;
         
        pfn = ref_function(&proc);
        if (pfn == NULL) {
             
            code = convert_transform(i_ctx_p, sepspace, &proc);
            if (code < 0)
                return code;
            if (code > 0) {
                *cont = 1;
                (*stage)++;
                return code;
            }
             
            op = osp;
            pfn = ref_function(op);
            pop (1);
        }
    } else {
         
        op = osp;
        pfn = ref_function(op);
        pop (1);
    }

    *stage = 0;
    if ((code = name_ref(imemory, (const byte *)"All", 3, &name_all, 0)) < 0)
        return code;
    if ((code = name_ref(imemory, (const byte *)"None", 4, &name_none, 0)) < 0)
        return code;
     
    code = array_get(imemory, sepspace, 1, &sname);
    if (code < 0)
        return code;

    if (r_has_type(&sname, t_string)) {
        code = name_from_string(imemory, &sname, &sname);
        if (code < 0)
            return code;
    }
    sep_type = ( name_eq(&sname, &name_all) ? SEP_ALL :
                 name_eq(&sname, &name_none) ? SEP_NONE : SEP_OTHER);

     
    pacs = gs_currentcolorspace(igs);

    cspace_old = istate->colorspace[0];
     
    code = gs_cspace_new_Separation(&pcs, pacs, imemory);
    if (code < 0)
        return code;
    pcs->params.separation.sep_type = sep_type;
    pcs->params.separation.sep_name = name_index(imemory, &sname);
    pcs->params.separation.get_colorname_string = gs_get_colorname_string;
    code = array_get(imemory, sepspace, 1, &proc);
    if (code < 0)
        return code;
    istate->colorspace[0].procs.special.separation.layer_name = proc;
    code = array_get(imemory, sepspace, 3, &proc);
    if (code < 0)
        return code;
    istate->colorspace[0].procs.special.separation.tint_transform = proc;
    if (code >= 0)
        code = gs_cspace_set_sepr_function(pcs, pfn);
    if (code >= 0)
        code = gs_setcolorspace(igs, pcs);
     
    rc_decrement_only_cs(pcs, "setseparationspace");
    if (code < 0) {
        istate->colorspace[0] = cspace_old;
        return code;
    }
    cc.pattern = 0x00;
    cc.paint.values[0] = 1.0;
    code = gs_setcolor(igs, &cc);
    return code;
}