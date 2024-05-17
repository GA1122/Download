pdf14_clist_update_params(pdf14_clist_device * pdev, const gs_gstate * pgs,
                          bool crop_blend_params,
                          gs_pdf14trans_params_t *group_params)
{
    gs_pdf14trans_params_t params = { 0 };
    gx_device * pcdev;
    int changed = 0;
    int code = 0;
    gs_composite_t *pct_new = NULL;

    params.crop_blend_params = crop_blend_params;

    params.pdf14_op = PDF14_SET_BLEND_PARAMS;
    if (pgs->blend_mode != pdev->blend_mode) {
        changed |= PDF14_SET_BLEND_MODE;
        params.blend_mode = pdev->blend_mode = pgs->blend_mode;
    }
    if (pgs->text_knockout != pdev->text_knockout) {
        changed |= PDF14_SET_TEXT_KNOCKOUT;
        params.text_knockout = pdev->text_knockout = pgs->text_knockout;
    }
    if (pgs->shape.alpha != pdev->shape) {
        changed |= PDF14_SET_SHAPE_ALPHA;
        params.shape.alpha = pdev->shape = pgs->shape.alpha;
    }
    if (pgs->opacity.alpha != pdev->opacity) {
        changed |= PDF14_SET_OPACITY_ALPHA;
        params.opacity.alpha = pdev->opacity = pgs->opacity.alpha;
    }
    if (pgs->overprint != pdev->overprint) {
        changed |= PDF14_SET_OVERPRINT;
        params.overprint = pdev->overprint = pgs->overprint;
    }
    if (pgs->overprint_mode != pdev->overprint_mode) {
        changed |= PDF14_SET_OVERPRINT_MODE;
        params.overprint_mode = pdev->overprint_mode = pgs->overprint_mode;
    }
    if (crop_blend_params) {
        params.ctm = group_params->ctm;
        params.bbox = group_params->bbox;
    }
    params.changed = changed;
     
    if (changed != 0) {
        code = gs_create_pdf14trans(&pct_new, &params, pgs->memory);
        if (code < 0) return code;
        code = dev_proc(pdev->target, create_compositor)
                    (pdev->target, &pcdev, pct_new, (gs_gstate *)pgs, pgs->memory, NULL);
        gs_free_object(pgs->memory, pct_new, "pdf14_clist_update_params");
    }
    return code;
}
