gx_dc_pattern_read(
    gx_device_color *       pdevc,
    const gs_gstate * pgs,
    const gx_device_color * prior_devc,
    const gx_device *       dev,
    int64_t                    offset,
    const byte *            data,
    uint                    size,
    gs_memory_t *           mem )
{
    gx_dc_serialized_tile_t buf;
    int size_b, size_c = -1;
    const byte *dp = data;
    int left = size;
    int64_t offset1 = offset;
    gx_color_tile *ptile;
    int code, l;
    tile_trans_clist_info_t trans_info = { { { 0 } } };
    int cache_space_needed;

    if (offset == 0) {
        pdevc->mask.id = gx_no_bitmap_id;
        pdevc->mask.m_tile = NULL;
        if (size == 0) {
             
            pdevc->type = &gx_dc_pattern;
            pdevc->colors.pattern.p_tile = NULL;
            pdevc->mask.id = gs_no_id;
            return 0;
        }
        if (size == sizeof(gs_id)) {
             
            gs_id id;  

            memcpy(&id, dp, sizeof(id));
            pdevc->type = &gx_dc_pattern;
            pdevc->mask.id = id;  
            return size;
        }
        if (sizeof(buf) > size) {
             
            return_error(gs_error_unregistered);  
        }
        memcpy(&buf, dp, sizeof(buf));
        dp += sizeof(buf);
        left -= sizeof(buf);
        offset1 += sizeof(buf);

        if ((buf.flags & TILE_USES_TRANSP) && !(buf.flags & TILE_IS_CLIST)){

            if (sizeof(buf) + sizeof(tile_trans_clist_info_t) > size) {
                return_error(gs_error_unregistered);  
            }

            memcpy(&trans_info, dp, sizeof(trans_info));
            dp += sizeof(trans_info);
            left -= sizeof(trans_info);
            offset1 += sizeof(trans_info);

                 
            cache_space_needed = trans_info.planestride > (0x7fffffff / 6) ? 0x7fff0000 :
                        trans_info.planestride * trans_info.n_chan;
        } else {
             
            cache_space_needed = buf.size_b + buf.size_c;
        }
        gx_pattern_cache_ensure_space((gs_gstate *)pgs, cache_space_needed);

        code = gx_pattern_cache_get_entry((gs_gstate *)pgs,  
                        buf.id, &ptile);
        if (code < 0)
            return code;
        gx_pattern_cache_update_used((gs_gstate *)pgs, cache_space_needed);
        ptile->bits_used = cache_space_needed;
        pdevc->type = &gx_dc_pattern;
        pdevc->colors.pattern.p_tile = ptile;
        ptile->id = buf.id;
        pdevc->mask.id = buf.id;
        ptile->step_matrix = buf.step_matrix;
        ptile->bbox = buf.bbox;
        ptile->depth = buf.flags & TILE_DEPTH_MASK;
        ptile->tiling_type = (buf.flags & TILE_TYPE_MASK)>>TILE_TYPE_SHIFT;
        ptile->is_simple = !!(buf.flags & TILE_IS_SIMPLE);
        ptile->has_overlap = !!(buf.flags & TILE_HAS_OVERLAP);
        ptile->blending_mode = buf.blending_mode;
        ptile->is_dummy = 0;

        if (!(buf.flags & TILE_IS_CLIST)) {

            if (buf.flags & TILE_USES_TRANSP){

                 

                ptile->ttrans = new_pattern_trans_buff(mem);
                 

                ptile->ttrans->height = trans_info.height;
                ptile->ttrans->n_chan = trans_info.n_chan;
                ptile->ttrans->has_tags = trans_info.has_tags;
                ptile->ttrans->pdev14 = NULL;
                ptile->ttrans->planestride = trans_info.planestride;
                ptile->ttrans->rect.p.x = trans_info.rect.p.x;
                ptile->ttrans->rect.p.y = trans_info.rect.p.y;
                ptile->ttrans->rect.q.x = trans_info.rect.q.x;
                ptile->ttrans->rect.q.y = trans_info.rect.q.y;
                ptile->ttrans->rowstride = trans_info.rowstride;
                ptile->ttrans->width = trans_info.width;
                pdevc->type = &gx_dc_pattern_trans;
                if_debug2m('v', pgs->memory,
                           "[v*] Reading trans tile from clist into cache, uid = %ld id = %ld \n",
                           ptile->uid.id, ptile->id);

                code = gx_dc_pattern_read_trans_buff(ptile, offset1, dp, left, mem);
                if (code < 0)
                    return code;
                return code + sizeof(buf)+sizeof(trans_info);

            } else {
                code = gx_dc_pattern_read_raster(ptile, &buf, offset1, dp, left, mem);
                if (code < 0)
                    return code;
                return code + sizeof(buf);
            }

        }

         
        size_b = buf.size_b;
        size_c = buf.size_c;
        ptile->tbits.size.x = size_b;  
        ptile->tbits.size.y = size_c;  
        {
            gs_gstate state;
            gs_pattern1_instance_t inst;

            memset(&state, 0, sizeof(state));
            memset(&inst, 0, sizeof(inst));
             
            state.device = (gx_device *)dev;  
            inst.templat.PaintType = 1;
            inst.size.x = buf.size.x;
            inst.size.y = buf.size.y;
            inst.saved = &state;
            inst.is_clist = !!(buf.flags & TILE_IS_CLIST);	 
            ptile->cdev = (gx_device_clist *)gx_pattern_accum_alloc(mem, mem,
                               &inst, "gx_dc_pattern_read");
            if (ptile->cdev == NULL)
                return_error(gs_error_VMerror);
            ptile->cdev->common.page_uses_transparency = !!(buf.flags & TILE_USES_TRANSP);
            code = dev_proc(&ptile->cdev->writer, open_device)((gx_device *)&ptile->cdev->writer);
            if (code < 0)
                return code;
        }
    } else {
        ptile = pdevc->colors.pattern.p_tile;

        if (ptile->ttrans != NULL)
            return gx_dc_pattern_read_trans_buff(ptile, offset1, dp, left, mem);

        if (ptile->cdev == NULL)
            return gx_dc_pattern_read_raster(ptile, NULL, offset1, dp, left, mem);

        size_b = ptile->tbits.size.x;
    }
    if (offset1 <= sizeof(buf) + size_b) {
        l = min(left, size_b - (offset1 - sizeof(buf)));
        code = clist_put_data(ptile->cdev, 0, offset1 - sizeof(buf), dp, l);
        if (code < 0)
            return code;
        l = code;
        left -= l;
        offset1 += l;
        dp += l;
        ptile->cdev->common.page_bfile_end_pos = offset1 - sizeof(buf);
    }
    if (left > 0) {
        l = left;
        code = clist_put_data(ptile->cdev, 1, offset1 - sizeof(buf) - size_b, dp, l);
        if (code < 0)
            return code;
        l = code;
        left -= l;
    }
    return size - left;
}
