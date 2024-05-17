gx_dc_pattern_read_raster(gx_color_tile *ptile, const gx_dc_serialized_tile_t *buf,
                          int64_t offset, const byte *data, uint size, gs_memory_t *mem)
{
    const byte *dp = data;
    int left = size;
    int64_t offset1 = offset;
    int size_b, size_c;

    if (buf != NULL) {
        size_b = buf->size_b;
        size_c = buf->size_c;
        ptile->tbits.data = gs_alloc_bytes(mem, size_b - sizeof(gx_strip_bitmap), "gx_dc_pattern_read_raster");
        if (ptile->tbits.data == NULL)
            return_error(gs_error_VMerror);
        if (size_c) {
            ptile->tmask.data = gs_alloc_bytes(mem, size_c - sizeof(gx_strip_bitmap), "gx_dc_pattern_read_raster");
            if (ptile->tmask.data == NULL)
                return_error(gs_error_VMerror);
        } else
            ptile->tmask.data = NULL;
        ptile->cdev = NULL;
    } else {
        size_b = gs_object_size(mem, ptile->tbits.data) + sizeof(gx_strip_bitmap);
        size_c = ptile->tmask.data != NULL ? gs_object_size(mem, ptile->tmask.data) + sizeof(gx_strip_bitmap) : 0;
    }
     
    if (offset1 <= sizeof(gx_dc_serialized_tile_t) + sizeof(gx_strip_bitmap)) {
        int l = min(sizeof(gx_strip_bitmap), left);
        byte *save = ptile->tbits.data;

        memcpy((byte*)&ptile->tbits + (offset1 - sizeof(gx_dc_serialized_tile_t)), dp, l);
        ptile->tbits.data = save;
        left -= l;
        offset1 += l;
        dp += l;
    }
    if (left == 0)
        return size;     
    if (offset1 <= sizeof(gx_dc_serialized_tile_t) + size_b) {
        int l = min(sizeof(gx_dc_serialized_tile_t) + size_b - offset1, left);

        memcpy(ptile->tbits.data +
                (offset1 - sizeof(gx_dc_serialized_tile_t) - sizeof(gx_strip_bitmap)), dp, l);
        left -= l;
        offset1 += l;
        dp += l;
    }
    if (left == 0 || size_c == 0)
        return size - left;
     
    if (offset1 <= sizeof(gx_dc_serialized_tile_t) + size_b + sizeof(gx_strip_bitmap)) {
        int l = min(sizeof(gx_dc_serialized_tile_t) + size_b + sizeof(gx_strip_bitmap) - offset1, left);
        byte *save = ptile->tmask.data;

        memcpy((byte*)&ptile->tmask + (offset1 - sizeof(gx_dc_serialized_tile_t) - size_b), dp, l);
        ptile->tmask.data = save;
        left -= l;
        offset1 += l;
        dp += l;
    }
    if (left == 0)
        return size;
    if (offset1 <= sizeof(gx_dc_serialized_tile_t) + size_b + size_c) {
        int l = min(sizeof(gx_dc_serialized_tile_t) + size_b + size_c - offset1, left);

        memcpy(ptile->tmask.data +
                (offset1 - sizeof(gx_dc_serialized_tile_t) - size_b - sizeof(gx_strip_bitmap)), dp, l);
        left -= l;
    }
    return size - left;
}
