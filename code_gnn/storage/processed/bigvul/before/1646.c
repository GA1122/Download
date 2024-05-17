static RELOC_PTRS_WITH(dc_pattern_reloc_ptrs, gx_device_color *cptr)
{
    gx_color_tile *tile = cptr->colors.pattern.p_tile;

    if (tile != 0) {
        uint index = tile->index;

        RELOC_TYPED_OFFSET_PTR(gx_device_color, colors.pattern.p_tile, index);
    }
    RELOC_USING(st_dc_pure_masked, vptr, size);
}
