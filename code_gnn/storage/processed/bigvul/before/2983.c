update_lop_for_pdf14(gs_gstate *pgs, const gx_drawing_color *pdcolor)
{
    bool hastrans = false;

     
    if (pdcolor != NULL)
    {
        if (gx_dc_is_pattern1_color(pdcolor) &&
            gx_pattern1_get_transptr(pdcolor) != NULL) {
            hastrans = true;
        } else if (gx_dc_is_pattern2_color(pdcolor)) {
             
            hastrans = true;
        }
    }
     
    if ((pgs->alpha != 0xFFFF) ||
        (pgs->blend_mode != BLEND_MODE_Normal && pgs->blend_mode != BLEND_MODE_Darken && pgs->blend_mode != BLEND_MODE_Lighten) ||
        (pgs->opacity.alpha != 1.0) ||
        (pgs->shape.alpha != 1.0) ||
        (hastrans))
    {
         
        pgs->log_op |= lop_pdf14;
    }
}
