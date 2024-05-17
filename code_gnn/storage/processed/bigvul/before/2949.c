pdf14_fill_mask(gx_device * orig_dev,
                     const byte * data, int dx, int raster, gx_bitmap_id id,
                     int x, int y, int w, int h,
                     const gx_drawing_color * pdcolor, int depth,
                     gs_logical_operation_t lop, const gx_clip_path * pcpath)
{
    gx_device *dev;
    pdf14_device *p14dev = (pdf14_device *)orig_dev;
    gx_device_clip cdev;
    gx_color_tile *ptile = NULL;
    int code = 0;
    gs_int_rect group_rect;
    gx_pattern_trans_t *fill_trans_buffer = NULL;
    bool has_pattern_trans = false;
    cmm_dev_profile_t *dev_profile;

    if (pdcolor == NULL)
        return_error(gs_error_unknownerror);	 
     
    if (gx_dc_is_pattern1_color(pdcolor)) {
        if( gx_pattern1_get_transptr(pdcolor) != NULL) {
            ptile = pdcolor->colors.pattern.p_tile;
             
             
            if (ptile->ttrans->n_chan-1 < 4) {
                ptile->ttrans->blending_procs = &rgb_blending_procs;
                ptile->ttrans->is_additive = true;
            } else {
                ptile->ttrans->blending_procs = &cmyk_blending_procs;
                ptile->ttrans->is_additive = false;
            }
             
            gx_set_pattern_procs_trans((gx_device_color*) pdcolor);
             
            if (ptile->has_overlap) {
                 
                ptile->ttrans->pat_trans_fill = &tile_rect_trans_blend;
            } else {
                 
                ptile->ttrans->pat_trans_fill = &tile_rect_trans_simple;
            }
             
            group_rect.p.x = x;
            group_rect.p.y = max(0,y);
            group_rect.q.x = x + w;
            group_rect.q.y = y + h;
            if (!(w <= 0 || h <= 0)) {
                code = pdf14_push_transparency_group(p14dev->ctx, &group_rect,
                     1, 0, 255,255, ptile->blending_mode, 0, 0,
                     ptile->ttrans->n_chan-1, false, NULL, NULL, NULL, NULL);
                if (code < 0)
                    return code;
                 
                fill_trans_buffer = new_pattern_trans_buff(p14dev->memory);
                pdf14_get_buffer_information((gx_device *) p14dev,
                                              fill_trans_buffer, NULL, false);
                 
                ptile->ttrans->fill_trans_buffer = fill_trans_buffer;
                has_pattern_trans = true;
            }
        }
    }
    if (pcpath != 0) {
        gx_make_clip_device_on_stack(&cdev, pcpath, orig_dev);
        dev = (gx_device *) & cdev;
    } else
        dev = orig_dev;
    if (depth > 1) {
         
        code = (*dev_proc(dev, copy_alpha))
            (dev, data, dx, raster, id, x, y, w, h,
             gx_dc_pure_color(pdcolor), depth);
    } else {
        code = pdcolor->type->fill_masked(pdcolor, data, dx, raster, id,
                                          x, y, w, h, dev, lop, false);
    }
    if (has_pattern_trans) {
        if (code >= 0)
            code = dev_proc(dev, get_profile)(dev,  &dev_profile);
        if (code >= 0)
            code = pdf14_pop_transparency_group(NULL, p14dev->ctx,
                                                p14dev->blend_procs,
                                                p14dev->color_info.num_components,
                                                dev_profile->device_profile[0],
                                                orig_dev);
        gs_free_object(p14dev->memory, ptile->ttrans->fill_trans_buffer,
                       "pdf14_fill_mask");
        ptile->ttrans->fill_trans_buffer = NULL;   
    }
    return code;
}
