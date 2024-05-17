gx_dc_pattern_trans_write_raster(gx_color_tile *ptile, int64_t offset, byte *data, uint *psize)
{
    int size, size_h;
    byte *dp = data;
    int left = *psize;
    int64_t offset1 = offset;
    unsigned char *ptr;

    size_h = sizeof(gx_dc_serialized_tile_t) + sizeof(tile_trans_clist_info_t);

     

    size = size_h + ptile->ttrans->n_chan * ptile->ttrans->planestride;
    if (ptile->ttrans->has_tags)
        size += ptile->ttrans->planestride;

     
    if (data == NULL) {
        *psize = size;
        return 0;
    }
    if (offset1 == 0) {  
        gx_dc_serialized_tile_t buf;
        tile_trans_clist_info_t trans_info;

        buf.id = ptile->id;
        buf.size.x = 0;  
        buf.size.y = 0;  
        buf.size_b = size - size_h;
        buf.size_c = 0;
        buf.flags = ptile->depth
                  | TILE_USES_TRANSP
                  | (ptile->tiling_type<<TILE_TYPE_SHIFT)
                  | (ptile->is_simple ? TILE_IS_SIMPLE : 0)
                  | (ptile->has_overlap ? TILE_HAS_OVERLAP : 0);
        buf.step_matrix = ptile->step_matrix;
        buf.bbox = ptile->bbox;
        buf.blending_mode = ptile->blending_mode;
        if (sizeof(buf) > left) {
             
            return_error(gs_error_unregistered);  
        }
        memcpy(dp, &buf, sizeof(buf));
        left -= sizeof(buf);
        dp += sizeof(buf);
        offset1 += sizeof(buf);

         
        trans_info.height = ptile->ttrans->height;
        trans_info.n_chan = ptile->ttrans->n_chan;
        trans_info.has_tags = ptile->ttrans->has_tags;
        trans_info.planestride = ptile->ttrans->planestride;
        trans_info.rect.p.x = ptile->ttrans->rect.p.x;
        trans_info.rect.p.y = ptile->ttrans->rect.p.y;
        trans_info.rect.q.x = ptile->ttrans->rect.q.x;
        trans_info.rect.q.y = ptile->ttrans->rect.q.y;
        trans_info.rowstride = ptile->ttrans->rowstride;
        trans_info.width = ptile->ttrans->width;

        if (sizeof(trans_info) > left) {
            return_error(gs_error_unregistered);  
        }
        memcpy(dp, &trans_info, sizeof(trans_info));
        left -= sizeof(trans_info);
        dp += sizeof(trans_info);
        offset1 += sizeof(trans_info);
    }

     

     
    if (offset1 <= size) {
         
        int u = min(size, left);

         
        ptr = ptile->ttrans->transbytes;
        memcpy(dp, ptr + (offset1 - size_h), u);
    }
    return 0;
}
