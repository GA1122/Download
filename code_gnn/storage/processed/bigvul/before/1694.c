gx_set_pattern_procs_trans(gx_device_color *pdevc)
{
    pdevc->type = &gx_dc_pattern_trans;
    return;
}
