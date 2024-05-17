gx_dc_is_pattern1_color(const gx_device_color *pdevc)
{
    return (pdevc->type == &gx_dc_pattern || pdevc->type == &gx_dc_pattern_trans);
}
