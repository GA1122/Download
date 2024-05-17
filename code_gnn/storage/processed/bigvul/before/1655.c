gs_dc_get_pattern_id(const gx_device_color *pdevc)
{
    if (!(gx_dc_is_pattern1_color(pdevc)))
        return gs_no_id;
    if (pdevc->colors.pattern.p_tile == NULL)
        return gs_no_id;
    return pdevc->colors.pattern.p_tile->id;
}
