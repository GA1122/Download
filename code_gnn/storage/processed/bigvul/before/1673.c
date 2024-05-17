gx_dc_is_pattern1_color_clist_based(const gx_device_color *pdevc)
{
    if (!(gx_dc_is_pattern1_color(pdevc)))
        return false;
    return gx_pattern_tile_is_clist(pdevc->colors.pattern.p_tile);
}
