gx_dc_pattern_save_dc(
    const gx_device_color * pdevc,
    gx_device_color_saved * psdc )
{
    psdc->type = pdevc->type;
    if (pdevc->ccolor_valid) {
        psdc->colors.pattern.id = pdevc->ccolor.pattern->pattern_id;
        psdc->colors.pattern.phase = pdevc->phase;
    } else {
         
        psdc->colors.pattern.id = gs_no_id;
        psdc->colors.pattern.phase.x = psdc->colors.pattern.phase.y = 0;
    }
}
