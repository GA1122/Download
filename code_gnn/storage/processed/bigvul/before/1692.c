gx_pattern_procs_istrans(gx_device_color *pdevc)
{
    return(pdevc->type == &gx_dc_pattern_trans);
}
