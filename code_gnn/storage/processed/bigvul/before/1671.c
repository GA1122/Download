gx_dc_devn_masked_load(gx_device_color * pdevc, const gs_gstate * pgs,
                       gx_device * dev, gs_color_select_t select)
{
    int code = (*gx_dc_type_data_devn.load) (pdevc, pgs, dev, select);

    if (code < 0)
        return code;
    FINISH_PATTERN_LOAD
}