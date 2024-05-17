gx_dc_pattern_write(
    const gx_device_color *         pdevc,
    const gx_device_color_saved *   psdc,
    const gx_device *               dev,
    int64_t                         offset,
    byte *                          data,
    uint *                          psize )
{
    gx_color_tile *ptile = pdevc->colors.pattern.p_tile;
    int size_b, size_c;
    byte *dp = data;
    int left = *psize;
    int64_t offset1 = offset;
    int code, l;

    if (ptile == NULL)
        return 0;
    if (psdc->type == pdevc->type) {
        if (psdc->colors.pattern.id == ptile->id) {
             
            return 1;  
        }
    }
    if (offset1 == 0 && left == sizeof(gs_id)) {
         
        gs_id id = ptile->id;  
        if_debug2m('v', dev->memory,
                   "[v*] Writing trans tile ID into clist, uid = %ld id = %ld \n",
                   ptile->uid.id, ptile->id);
        memcpy(dp, &ptile->id, sizeof(id));
        *psize = sizeof(gs_id);
        return 0;
    }

     
        if (ptile->ttrans != NULL) {
            if_debug2m('v', dev->memory,
                       "[v*] Writing trans tile into clist, uid = %ld id = %ld \n",
                       ptile->uid.id, ptile->id);
            return gx_dc_pattern_trans_write_raster(ptile, offset, data, psize);
        }

    if (ptile->cdev == NULL)
        return gx_dc_pattern_write_raster(ptile, offset, data, psize, dev);
     
    size_b = clist_data_size(ptile->cdev, 0);
    if (size_b < 0)
        return_error(gs_error_unregistered);
    size_c = clist_data_size(ptile->cdev, 1);
    if (size_c < 0)
        return_error(gs_error_unregistered);
    if (data == NULL) {
        *psize = sizeof(gx_dc_serialized_tile_t) + size_b + size_c;
        return 0;
    }
    if (offset1 == 0) {  
        gx_dc_serialized_tile_t buf;

        buf.id = ptile->id;
        buf.size.x = ptile->cdev->common.width;
        buf.size.y = ptile->cdev->common.height;
        buf.size_b = size_b;
        buf.size_c = size_c;
        buf.step_matrix = ptile->step_matrix;
        buf.bbox = ptile->bbox;
        buf.flags = ptile->depth
                  | TILE_IS_CLIST
                  | (ptile->tiling_type<<TILE_TYPE_SHIFT)
                  | (ptile->is_simple ? TILE_IS_SIMPLE : 0)
                  | (ptile->has_overlap ? TILE_HAS_OVERLAP : 0)
                  | (ptile->cdev->common.page_uses_transparency ? TILE_USES_TRANSP : 0);
        buf.blending_mode = ptile->blending_mode;     
        if (sizeof(buf) > left) {
             
            return_error(gs_error_unregistered);  
        }
        memcpy(dp, &buf, sizeof(gx_dc_serialized_tile_t));
        left -= sizeof(buf);
        dp += sizeof(buf);
        offset1 += sizeof(buf);
    }
    if (offset1 <= sizeof(gx_dc_serialized_tile_t) + size_b) {
        l = min(left, size_b - (offset1 - sizeof(gx_dc_serialized_tile_t)));
        code = clist_get_data(ptile->cdev, 0, offset1 - sizeof(gx_dc_serialized_tile_t), dp, l);
        if (code < 0)
            return code;
        left -= l;
        offset1 += l;
        dp += l;
    }
    if (left > 0) {
        l = min(left, size_c - (offset1 - sizeof(gx_dc_serialized_tile_t) - size_b));
        code = clist_get_data(ptile->cdev, 1, offset1 - sizeof(gx_dc_serialized_tile_t) - size_b, dp, l);
        if (code < 0)
            return code;
    }
    return 0;
}
