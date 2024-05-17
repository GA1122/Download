pdf14_stroke_path(gx_device *dev, const	gs_gstate	*pgs,
                             gx_path *ppath, const gx_stroke_params *params,
                             const gx_drawing_color *pdcolor,
                             const gx_clip_path *pcpath)
{
    gs_gstate new_pgs = *pgs;

    update_lop_for_pdf14(&new_pgs, pdcolor);
    pdf14_set_marking_params(dev, pgs);
    return gx_default_stroke_path(dev, &new_pgs, ppath, params, pdcolor,
                                  pcpath);
}
