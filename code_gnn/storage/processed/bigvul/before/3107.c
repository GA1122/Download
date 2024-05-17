static int is_same_colorspace(i_ctx_t * i_ctx_p, ref *space1, ref *space2, bool isCIE)
{
    PS_colour_space_t *oldcspace = 0, *newcspace = 0;
    ref oldspace, *poldspace = &oldspace, newspace, *pnewspace = &newspace;
    int code, CIESubst;

     
    oldspace.tas.type_attrs = 0;
    oldspace.tas.type_attrs = 0;

    ref_assign(pnewspace, space1);
    ref_assign(poldspace, space2);

    do {
        if (r_type(poldspace) != r_type(pnewspace))
            return 0;

        code = get_space_object(i_ctx_p, poldspace, &oldcspace);
        if (code < 0)
            return 0;

        code = get_space_object(i_ctx_p, pnewspace, &newcspace);
        if (code < 0)
            return 0;

         
        if (strcmp(oldcspace->name, newcspace->name) != 0)
            return 0;

         
        if (!oldcspace->compareproc(i_ctx_p, poldspace, pnewspace))
            return 0;

         

        if ( name_is_device_color(newcspace->name) ){
            if ( gs_color_space_is_CIE(gs_currentcolorspace_inline(i_ctx_p->pgs)) ){
                if ( !isCIE ) return 0;  
            } else {
                if ( isCIE ) return 0;  
            }
        }

         
        if (!oldcspace->alternateproc)
            break;

         
        code = oldcspace->alternateproc(i_ctx_p, poldspace, &poldspace, &CIESubst);
        if (code < 0)
            return 0;

        code = newcspace->alternateproc(i_ctx_p, pnewspace, &pnewspace, &CIESubst);
        if (code < 0)
            return 0;
    }
    while(1);

    return 1;
}
