gs_cspace_build_Pattern1(gs_color_space ** ppcspace,
                    gs_color_space * pbase_cspace, gs_memory_t * pmem)
{
    gs_color_space *pcspace = 0;

    if (pbase_cspace != 0) {
        if (gs_color_space_num_components(pbase_cspace) < 0)          
            return_error(gs_error_rangecheck);
    }
    pcspace = gs_cspace_alloc(pmem, &gs_color_space_type_Pattern);
    if (pcspace == NULL)
        return_error(gs_error_VMerror);
    if (pbase_cspace != 0) {
        pcspace->params.pattern.has_base_space = true;
         
        pcspace->base_space = pbase_cspace;
    } else
        pcspace->params.pattern.has_base_space = false;
    *ppcspace = pcspace;
    return 0;
}
