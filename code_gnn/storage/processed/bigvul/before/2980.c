pdf14_tile_pattern_fill(gx_device * pdev, const gs_gstate * pgs,
                        gx_path * ppath, const gx_fill_params * params,
                        const gx_device_color * pdevc,
                        const gx_clip_path * pcpath)
{
    int code;
    gs_gstate *pgs_noconst = (gs_gstate *)pgs;  
    gs_fixed_rect clip_box;
    gs_fixed_rect outer_box;
    pdf14_device * p14dev = (pdf14_device *)pdev;
    gs_int_rect rect;
    gx_clip_rect *curr_clip_rect;
    gx_color_tile *ptile = NULL;
    int k;
    gx_pattern_trans_t *fill_trans_buffer = NULL;
    gs_int_point phase;   
    int n_chan_tile;
    gx_clip_path cpath_intersection;
    gx_path path_ttrans;
    gs_blend_mode_t blend_mode;

    if (ppath == NULL)
        return_error(gs_error_unknownerror);	 
    if (pcpath != NULL) {
        code = gx_cpath_init_local_shared_nested(&cpath_intersection, pcpath, ppath->memory, 1);
    } else {
        (*dev_proc(pdev, get_clipping_box)) (pdev, &clip_box);
        gx_cpath_init_local(&cpath_intersection, ppath->memory);
        code = gx_cpath_from_rectangle(&cpath_intersection, &clip_box);
    }
    if (code < 0)
        return code;
    code = gx_cpath_intersect_with_params(&cpath_intersection, ppath,
                                          params->rule, pgs_noconst, params);
    if (code < 0)
        return code;
     
    ptile = pdevc->colors.pattern.p_tile;
    if (ptile->ttrans != NULL)
    {
        if ((cpath_intersection.outer_box.p.x < 0) ||
            (cpath_intersection.outer_box.p.y < 0) ||
            (cpath_intersection.outer_box.q.x > int2fixed(ptile->ttrans->width)) ||
            (cpath_intersection.outer_box.q.y > int2fixed(ptile->ttrans->height)))
        {
             
        } else {
             
            gx_path_init_local(&path_ttrans, ppath->memory);
            code = gx_path_add_rectangle(&path_ttrans,
                                         int2fixed(ptile->ttrans->rect.p.x),
                                         int2fixed(ptile->ttrans->rect.p.y),
                                         int2fixed(ptile->ttrans->rect.q.x),
                                         int2fixed(ptile->ttrans->rect.q.y));
            if (code < 0)
                return code;
            code = gx_cpath_intersect(&cpath_intersection, &path_ttrans,
                                      params->rule, pgs_noconst);
            if (code < 0)
                return code;
        }
    }
     
    if (ppath != NULL && code >= 0) {
        pdf14_device save_pdf14_dev;		 

        gx_cpath_outer_box(&cpath_intersection, &outer_box);
        rect.p.x = fixed2int(outer_box.p.x);
        rect.p.y = fixed2int(outer_box.p.y);
        rect.q.x = fixed2int_ceiling(outer_box.q.x);
        rect.q.y = fixed2int_ceiling(outer_box.q.y);

         
        if (ptile->cdev == NULL) {
            if (ptile->ttrans == NULL)
                return_error(gs_error_unknownerror);	 
            n_chan_tile = ptile->ttrans->n_chan;
        } else {
            n_chan_tile = ptile->cdev->common.color_info.num_components+1;
        }
        blend_mode = ptile->blending_mode;
        memcpy(&save_pdf14_dev, p14dev, sizeof(pdf14_device));
        code = pdf14_push_transparency_group(p14dev->ctx, &rect, 1, 0, 255,255,
                                             blend_mode, 0, 0, n_chan_tile-1,
                                             false, NULL, NULL, pgs_noconst,
                                             pdev);
        if (code < 0)
            return code;

         
        if (ptile->cdev == NULL) {
            if (n_chan_tile-1 < 4) {
                ptile->ttrans->blending_procs = &rgb_blending_procs;
                ptile->ttrans->is_additive = true;
            } else {
                ptile->ttrans->blending_procs = &cmyk_blending_procs;
                ptile->ttrans->is_additive = false;
            }
        }
         
         
        if (ptile->cdev == NULL) {
            fill_trans_buffer = new_pattern_trans_buff(pgs->memory);
            pdf14_get_buffer_information(pdev, fill_trans_buffer, NULL, false);
             
            if (ptile->has_overlap) {
                 
                ptile->ttrans->pat_trans_fill = &tile_rect_trans_blend;
            } else {
                 
                ptile->ttrans->pat_trans_fill = &tile_rect_trans_simple;
            }
             
            phase.x = pdevc->phase.x;
            phase.y = pdevc->phase.y;
            if (cpath_intersection.rect_list->list.head != NULL){
                curr_clip_rect = cpath_intersection.rect_list->list.head->next;
                for( k = 0; k < cpath_intersection.rect_list->list.count && code >= 0; k++){
                    if_debug5m('v', pgs->memory,
                               "[v]pdf14_tile_pattern_fill, (%d, %d), %d x %d pat_id %d \n",
                               curr_clip_rect->xmin, curr_clip_rect->ymin,
                               curr_clip_rect->xmax-curr_clip_rect->xmin,
                               curr_clip_rect->ymax-curr_clip_rect->ymin, (int)ptile->id);
                    code = gx_trans_pattern_fill_rect(curr_clip_rect->xmin, curr_clip_rect->ymin,
                                                      curr_clip_rect->xmax, curr_clip_rect->ymax, ptile,
                                                      fill_trans_buffer, phase, pdev, pdevc);
                    curr_clip_rect = curr_clip_rect->next;
                }
            } else if (cpath_intersection.rect_list->list.count == 1) {
                 
                if_debug5m('v', pgs->memory,
                           "[v]pdf14_tile_pattern_fill, (%d, %d), %d x %d pat_id %d \n",
                           cpath_intersection.rect_list->list.single.xmin,
                           cpath_intersection.rect_list->list.single.ymin,
                           cpath_intersection.rect_list->list.single.xmax-
                              cpath_intersection.rect_list->list.single.xmin,
                           cpath_intersection.rect_list->list.single.ymax-
                              cpath_intersection.rect_list->list.single.ymin,
                           (int)ptile->id);
                code = gx_trans_pattern_fill_rect(cpath_intersection.rect_list->list.single.xmin,
                                                  cpath_intersection.rect_list->list.single.ymin,
                                                  cpath_intersection.rect_list->list.single.xmax,
                                                  cpath_intersection.rect_list->list.single.ymax,
                                                  ptile, fill_trans_buffer, phase, pdev, pdevc);
            }
        } else {
             
            gx_device *dev;
            gx_device_clip clipdev;

            gx_make_clip_device_on_stack(&clipdev, &cpath_intersection, pdev);
            dev = (gx_device *)&clipdev;
            phase.x = pdevc->phase.x;
            phase.y = pdevc->phase.y;
            code = gx_trans_pattern_fill_rect(rect.p.x, rect.p.y, rect.q.x, rect.q.y,
                                              ptile, fill_trans_buffer, phase,
                                              dev, pdevc);

        }
         
        p14dev->pclist_device = NULL;
        if (code < 0)
            return code;

         
        if (fill_trans_buffer != NULL) {
            gs_free_object(pgs->memory, fill_trans_buffer, "pdf14_tile_pattern_fill");
            ptile->ttrans->fill_trans_buffer = NULL;   
        }
         
        code = pdf14_pop_transparency_group(pgs_noconst, p14dev->ctx,
                                            p14dev->blend_procs,
                                            p14dev->color_info.num_components,
                                            p14dev->icc_struct->device_profile[0],
                                            pdev);
        memcpy(p14dev, &save_pdf14_dev, sizeof(pdf14_device));
        p14dev->pclist_device = NULL;
    }
    return code;
}
