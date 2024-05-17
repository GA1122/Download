gx_dc_pattern_write_raster(gx_color_tile *ptile, int64_t offset, byte *data,
                           uint *psize, const gx_device *dev)
{
    int size_b, size_c;
    byte *dp = data;
    int left = *psize;
    int64_t offset1 = offset;

    size_b = sizeof(gx_strip_bitmap) +
         ptile->tbits.size.y * ptile->tbits.raster * ptile->tbits.num_planes;
    size_c = ptile->tmask.data ? sizeof(gx_strip_bitmap) + ptile->tmask.size.y * ptile->tmask.raster : 0;
    if (data == NULL) {
        *psize = sizeof(gx_dc_serialized_tile_t) + size_b + size_c;
        return 0;
    }
    if (offset1 == 0) {  
#if defined(DEBUG) || defined(PACIFY_VALGRIND) || defined(MEMENTO)
        gx_dc_serialized_tile_t buf = { 0 };
        gx_strip_bitmap buf1 = { 0 };
#else
        gx_dc_serialized_tile_t buf;
        gx_strip_bitmap buf1;
#endif

        buf.id = ptile->id;
        buf.size.x = 0;  
        buf.size.y = 0;  
        buf.size_b = size_b;
        buf.size_c = size_c;
        buf.step_matrix = ptile->step_matrix;
        buf.bbox = ptile->bbox;
        buf.flags = ptile->depth
                  | (ptile->tiling_type<<TILE_TYPE_SHIFT)
                  | (ptile->is_simple ? TILE_IS_SIMPLE : 0)
                  | (ptile->has_overlap ? TILE_HAS_OVERLAP : 0);
        if (sizeof(buf) > left) {
             
            return_error(gs_error_unregistered);  
        }
        memcpy(dp, &buf, sizeof(buf));
        left -= sizeof(buf);
        dp += sizeof(buf);
        offset1 += sizeof(buf);

        buf1 = ptile->tbits;
        buf1.data = NULL;  
        if (sizeof(buf1) > left) {
             
            return_error(gs_error_unregistered);  
        }
        memcpy(dp, &buf1, sizeof(buf1));
        left -= sizeof(buf1);
        dp += sizeof(buf1);
        offset1 += sizeof(buf1);
    }
    if (offset1 <= sizeof(gx_dc_serialized_tile_t) + size_b) {
        int l = min((size_b - sizeof(gx_strip_bitmap)) - (offset1 - sizeof(gx_dc_serialized_tile_t) -  sizeof(gx_strip_bitmap)), left);

        memcpy(dp, ptile->tbits.data + (offset1 - sizeof(gx_dc_serialized_tile_t) -  sizeof(gx_strip_bitmap)), l);
        left -= l;
        dp += l;
        offset1 += l;
    }
    if (left == 0)
        return 0;
    if (size_c == 0)
        return 0;
    if (offset1 <= sizeof(gx_dc_serialized_tile_t) + size_b + sizeof(gx_strip_bitmap)) {
        gx_strip_bitmap buf;

        if (left < sizeof(buf))
            return_error(gs_error_unregistered);  
        buf = ptile->tmask;
        buf.data = NULL;  
        memcpy(dp, &buf, sizeof(buf));
        left -= sizeof(buf);
        dp += sizeof(buf);
        offset1 += sizeof(buf);
    }
    if (offset1 <= sizeof(gx_dc_serialized_tile_t) + size_b + size_c) {
        int l = min(size_c - sizeof(gx_strip_bitmap), left);

        memcpy(dp, ptile->tmask.data + (offset1 - sizeof(gx_dc_serialized_tile_t) - size_b - sizeof(gx_strip_bitmap)), l);
    }
    return 0;
}
