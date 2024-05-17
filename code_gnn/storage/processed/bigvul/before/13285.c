pdf14_clist_fill_path(gx_device	*dev, const gs_gstate *pgs,
                           gx_path *ppath, const gx_fill_params *params,
                           const gx_drawing_color *pdcolor,
                           const gx_clip_path *pcpath)
{
    pdf14_clist_device * pdev = (pdf14_clist_device *)dev;
    gs_gstate new_pgs = *pgs;
    int code;
    gs_pattern2_instance_t *pinst = NULL;
    gx_device_forward * fdev = (gx_device_forward *)dev;
    cmm_dev_profile_t *dev_profile, *fwd_profile;
    gsicc_rendering_param_t render_cond;
    cmm_profile_t *icc_profile_fwd, *icc_profile_dev;

    code = dev_proc(dev, get_profile)(dev,  &dev_profile);
    if (code < 0)
        return code;
    code = dev_proc(fdev->target, get_profile)(fdev->target,  &fwd_profile);
    if (code < 0)
        return code;

    gsicc_extract_profile(GS_UNKNOWN_TAG, fwd_profile, &icc_profile_fwd,
                          &render_cond);
    gsicc_extract_profile(GS_UNKNOWN_TAG, dev_profile, &icc_profile_dev,
                          &render_cond);

     
    code = pdf14_clist_update_params(pdev, pgs, false, NULL);
    if (code < 0)
        return code;
     

    if (pdcolor != NULL && gx_dc_is_pattern2_color(pdcolor)) {
        pinst =
            (gs_pattern2_instance_t *)pdcolor->ccolor.pattern;
           pinst->saved->has_transparency = true;
            
           pinst->saved->trans_device = dev;
    }
    update_lop_for_pdf14(&new_pgs, pdcolor);
    new_pgs.trans_device = dev;
    new_pgs.has_transparency = true;
    code = gx_forward_fill_path(dev, &new_pgs, ppath, params, pdcolor, pcpath);
    new_pgs.trans_device = NULL;
    new_pgs.has_transparency = false;
    if (pinst != NULL){
        pinst->saved->trans_device = NULL;
    }
    return code;
}
