ENUM_PTRS_WITH(dc_pattern_enum_ptrs, gx_device_color *cptr)
{
    return ENUM_USING(st_dc_pure_masked, vptr, size, index - 1);
}
