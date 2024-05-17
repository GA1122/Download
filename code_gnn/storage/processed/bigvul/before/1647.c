static RELOC_PTRS_WITH(dc_masked_reloc_ptrs, gx_device_color *cptr)
{
    gx_color_tile *mask = cptr->mask.m_tile;

    RELOC_SUPER(gx_device_color, st_client_color, ccolor);
    if (mask != 0) {
        uint index = mask->index;

        RELOC_TYPED_OFFSET_PTR(gx_device_color, mask.m_tile, index);
    }
}
