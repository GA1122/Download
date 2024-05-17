gx_set_pattern_procs_standard(gx_device_color *pdevc)
{
    pdevc->type = &gx_dc_pattern;
    return;
}
