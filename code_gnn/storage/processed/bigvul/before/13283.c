pdf14_clist_begin_typed_image(gx_device	* dev, const gs_gstate * pgs,
                           const gs_matrix *pmat, const gs_image_common_t *pic,
                           const gs_int_rect * prect,
                           const gx_drawing_color * pdcolor,
                           const gx_clip_path * pcpath, gs_memory_t * mem,
                           gx_image_enum_common_t ** pinfo)
{
    pdf14_clist_device * pdev = (pdf14_clist_device *)dev;
    int code;
    gs_gstate * pgs_noconst = (gs_gstate *)pgs;  
    const gs_image_t *pim = (const gs_image_t *)pic;
    gx_image_enum *penum;
    gx_color_tile *ptile;
    gs_rect bbox_in, bbox_out;
    gs_transparency_group_params_t tgp;
     
    code = pdf14_clist_update_params(pdev, pgs, false, NULL);
    if (code < 0)
        return code;
     
     
    pgs_noconst->has_transparency = true;
    pgs_noconst->trans_device = dev;

     
    if (pim->ImageMask) {
        if (pdcolor != NULL && gx_dc_is_pattern1_color(pdcolor)) {
            if( gx_pattern1_get_transptr(pdcolor) != NULL){
                 if (dev->procs.begin_image != pdf14_clist_begin_image) {
                    ptile = pdcolor->colors.pattern.p_tile;
                     
                     
                    if (ptile->ttrans->n_chan-1 < 4) {
                        ptile->ttrans->blending_procs = &rgb_blending_procs;
                        ptile->ttrans->is_additive = true;
                    } else {
                        ptile->ttrans->blending_procs = &cmyk_blending_procs;
                        ptile->ttrans->is_additive = false;
                    }
                     
                    ptile->blending_mode = pgs->blend_mode;
                     
                     
                    code = gx_default_begin_typed_image(dev, pgs, pmat, pic,
                                                        prect, pdcolor,
                                                        pcpath, mem, pinfo);
                    if (code < 0)
                        return code;

                    penum = (gx_image_enum *) *pinfo;
                     
                    bbox_in.p.x = 0;
                    bbox_in.p.y = 0;
                    bbox_in.q.x = pim->Width;
                    bbox_in.q.y = pim->Height;
                    code = gs_bbox_transform_inverse(&bbox_in, &(pim->ImageMatrix),
                                                     &bbox_out);
                    if (code < 0) return code;
                     
                    if_debug0m('v', pgs->memory, "[v]Pushing special trans group for image\n");
                    tgp.Isolated = true;
                    tgp.Knockout = false;
                    tgp.mask_id = 0;
                    tgp.image_with_SMask = false;
                    tgp.idle = false;
                    tgp.iccprofile = NULL;
                    tgp.icc_hashcode = 0;
                    tgp.group_color_numcomps = ptile->ttrans->n_chan-1;
                    tgp.ColorSpace = NULL;
                     
                    gs_begin_transparency_group((gs_gstate *) pgs_noconst, &tgp,
                                                &bbox_out);
                    ptile->ttrans->image_render = penum->render;
                    penum->render = &pdf14_pattern_trans_render;
                    ptile->trans_group_popped = false;
                    pgs_noconst->has_transparency = false;
                    pgs_noconst->trans_device = NULL;
                    return code;
                }
            }
        }
    }
     
    code = gx_forward_begin_typed_image(dev, pgs, pmat,
                            pic, prect, pdcolor, pcpath, mem, pinfo);
    if (code < 0){
        code = gx_default_begin_typed_image(dev, pgs, pmat, pic, prect,
                                        pdcolor, pcpath, mem, pinfo);
        pgs_noconst->has_transparency = false;
        pgs_noconst->trans_device = NULL;
        return code;
    } else {
        pgs_noconst->has_transparency = false;
        pgs_noconst->trans_device = NULL;
        return code;
    }
}
