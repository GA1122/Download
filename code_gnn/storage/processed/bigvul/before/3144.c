static int setdevicenspace(i_ctx_t * i_ctx_p, ref *devicenspace, int *stage, int *cont, int CIESubst)
{
    os_ptr  op = osp;    
    int code = 0, num_components, i;
    ref namesarray, proc, sname, tname, sref;
    ref_colorspace cspace_old;
    gs_color_space *pcs;
    gs_color_space * pacs;
    gs_function_t *pfn = NULL;
    gs_separation_name *names;
    gs_client_color cc;

    if (i_ctx_p->language_level < 3)
        return_error(gs_error_undefined);

    *cont = 0;
    if ((*stage) == 2) {
        if (r_size(devicenspace) == 5) {
             
        	ref *colorants;

            code = array_get(imemory, devicenspace, 4, &sref);
            if (code < 0)
                return code;
            if (!r_has_type(&sref, t_dictionary)) {
                *stage = 0;
                return 0;
            }
            if (dict_find_string(&sref, "Colorants", &colorants) <= 0) {
                *stage = 0;
                return 0;
            }
            if (!r_has_type(colorants, t_dictionary)) {
                *stage = 0;
                return 0;
            }
            *stage = 3;
            *cont = 1;
            check_estack(5);
            push_mark_estack(es_other, colour_cleanup);
            esp++;
             
            make_int(esp, dict_first(colorants));
            esp++;
             
            make_int(esp, 0);
            esp++;
             
            ref_assign(esp, colorants);
            push_op_estack(devicencolorants_cont);
            return o_push_estack;
        } else {
            *stage = 0;
            return 0;
        }
    }
    if ((*stage) == 3) {
        *stage = 0;
        return 0;
    }
    if ((*stage) == 0) {
        code = array_get(imemory, devicenspace, 3, &proc);
        if (code < 0)
            return code;
        pfn = ref_function(&proc);
        if (pfn == NULL) {
             
            code = convert_transform(i_ctx_p, devicenspace, &proc);
            if (code < 0)
                return code;
            if (code > 0) {
                *cont = 1;
                *stage = 1;
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

    *stage = 2;

    code = array_get(imemory, devicenspace, 1, &namesarray);
    if (code < 0)
        return code;
    num_components = r_size(&namesarray);
     
    pacs = gs_currentcolorspace(igs);

    if (num_components == 1) {
        array_get(imemory, &namesarray, (long)0, &sname);
        switch (r_type(&sname)) {
            case t_string:
                tname = sname;
                break;
            case t_name:
                name_string_ref(imemory, &sname, &tname);
                break;
            default:
                return_error(gs_error_typecheck);
                break;
        }
        if (strncmp((const char *)tname.value.const_bytes, "All", 3) == 0 && r_size(&tname) == 3) {
            separation_type sep_type;

             
            sep_type = SEP_ALL;

             
            pacs = gs_currentcolorspace(igs);

            cspace_old = istate->colorspace[0];
             
            code = gs_cspace_new_Separation(&pcs, pacs, imemory);
            if (code < 0)
                return code;
            pcs->params.separation.sep_type = sep_type;
            pcs->params.separation.sep_name = name_index(imemory, &sname);
            pcs->params.separation.get_colorname_string = gs_get_colorname_string;
            code = array_get(imemory, &namesarray, (long)0, &sname);
            if (code < 0)
                return code;
            istate->colorspace[0].procs.special.separation.layer_name = sname;
            code = array_get(imemory, devicenspace, 3, &proc);
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
    }
    code = gs_cspace_new_DeviceN(&pcs, num_components, pacs, imemory);
    if (code < 0)
        return code;
    names = pcs->params.device_n.names;
    pcs->params.device_n.get_colorname_string = gs_get_colorname_string;

     
    {
        uint i;
        ref sname;

        for (i = 0; i < num_components; ++i) {
            array_get(imemory, &namesarray, (long)i, &sname);
            switch (r_type(&sname)) {
                case t_string:
                    code = name_from_string(imemory, &sname, &sname);
                    if (code < 0) {
                        rc_decrement_cs(pcs, "setdevicenspace");
                        return code;
                    }
                     
                case t_name:
                    names[i] = name_index(imemory, &sname);
                    break;
                default:
                    rc_decrement_cs(pcs, "setdevicenspace");
                    return_error(gs_error_typecheck);
            }
        }
    }

     

    cspace_old = istate->colorspace[0];
    istate->colorspace[0].procs.special.device_n.layer_names = namesarray;
    code = array_get(imemory, devicenspace, 3, &proc);
    if (code < 0)
        return code;
    istate->colorspace[0].procs.special.device_n.tint_transform = proc;
    code = gs_cspace_set_devn_function(pcs, pfn);
    if (code < 0) {
        return code;
    }
    code = gs_setcolorspace(igs, pcs);
     
    rc_decrement_only_cs(pcs, "setdevicenspace");
    if (code < 0) {
        istate->colorspace[0] = cspace_old;
        return code;
    }

    cc.pattern = 0x00;
    for (i=0;i<num_components;i++)
        cc.paint.values[i] = 1.0;
    code = gs_setcolor(igs, &cc);
    *cont = 1;
    return code;
}