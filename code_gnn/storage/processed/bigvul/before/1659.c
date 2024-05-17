gs_makepixmappattern(
                        gs_client_color * pcc,
                        const gs_depth_bitmap * pbitmap,
                        bool mask,
                        const gs_matrix * pmat,
                        long id,
                        gs_color_space * pcspace,
                        uint white_index,
                        gs_gstate * pgs,
                        gs_memory_t * mem
)
{

    gs_pattern1_template_t pat;
    pixmap_info *ppmap;
    gs_matrix mat, smat;
    int code;

     
    if ((mask) || (pcspace == 0)) {
        if (pbitmap->pix_depth != 1)
            return_error(gs_error_rangecheck);
        pcspace = 0;
    } else if (gs_color_space_get_index(pcspace) != gs_color_space_index_Indexed)
        return_error(gs_error_rangecheck);
    if (pbitmap->num_comps != 1)
        return_error(gs_error_rangecheck);

     
    if (mem == 0)
        mem = gs_gstate_memory(pgs);
    ppmap = gs_alloc_struct(mem,
                            pixmap_info,
                            &st_pixmap_info,
                            "makepximappattern"
        );
    if (ppmap == 0)
        return_error(gs_error_VMerror);
    ppmap->bitmap = *pbitmap;
    ppmap->pcspace = pcspace;
    ppmap->white_index = white_index;

     
    gs_pattern1_init(&pat);
    uid_set_UniqueID(&pat.uid, (id == no_UniqueID) ? gs_next_ids(mem, 1) : id);
    pat.PaintType = (mask ? 2 : 1);
    pat.TilingType = 1;
    pat.BBox.p.x = 0;
    pat.BBox.p.y = 0;
    pat.BBox.q.x = pbitmap->size.x;
    pat.BBox.q.y = pbitmap->size.y;
    pat.XStep = (float)pbitmap->size.x;
    pat.YStep = (float)pbitmap->size.y;
    pat.PaintProc = (mask ? pixmap_remap_mask_pattern : pixmap_remap_image_pattern);
    pat.client_data = ppmap;

     
    gs_currentmatrix(pgs, &smat);
    gs_make_identity(&mat);
    gs_setmatrix(pgs, &mat);

     
    if (pmat == NULL)
        pmat = &mat;
    if ((code = gs_makepattern(pcc, &pat, pmat, pgs, mem)) != 0)
        gs_free_object(mem, ppmap, "makebitmappattern_xform");
    else {
         
        gs_pattern1_instance_t *pinst =
            (gs_pattern1_instance_t *)pcc->pattern;

        if (!mask && (white_index >= (1 << pbitmap->pix_depth)))
            pinst->uses_mask = false;

         
        ppmap->free_proc = pinst->rc.free;
        pinst->rc.free = free_pixmap_pattern;

         
        code = gs_setgray(pinst->saved, 0.0);

    }
    gs_setmatrix(pgs, &smat);
    return code;
}
