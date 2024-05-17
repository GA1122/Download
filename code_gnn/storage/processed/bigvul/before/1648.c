static RELOC_PTRS_WITH(dc_colored_masked_reloc_ptrs, gx_device_color *cptr)
{
    RELOC_SUPER(gx_device_color, st_client_color, ccolor);
    if (cptr->colors.colored.c_ht != 0) {
        RELOC_PTR(gx_device_color, colors.colored.c_ht);
    }
}
