int pixmap_high_level_pattern(gs_gstate * pgs)
{
    gs_matrix m;
    gs_rect bbox;
    gs_fixed_rect clip_box;
    int code;
    gx_device_color *pdc = gs_currentdevicecolor_inline(pgs);
    const gs_client_pattern *ppat = gs_getpattern(&pdc->ccolor);
    gs_color_space *pcs;
    gs_pattern1_instance_t *pinst =
        (gs_pattern1_instance_t *)gs_currentcolor(pgs)->pattern;
    const pixmap_info *ppmap = ppat->client_data;

    code = gx_pattern_cache_add_dummy_entry(pgs, pinst, pgs->device->color_info.depth);
    if (code < 0)
        return code;

    code = gs_gsave(pgs);
    if (code < 0)
        return code;

    dev_proc(pgs->device, get_initial_matrix)(pgs->device, &m);
    gs_setmatrix(pgs, &m);
    code = gs_bbox_transform(&ppat->BBox, &ctm_only(pgs), &bbox);
    if (code < 0) {
        gs_grestore(pgs);
            return code;
    }
    clip_box.p.x = float2fixed(bbox.p.x);
    clip_box.p.y = float2fixed(bbox.p.y);
    clip_box.q.x = float2fixed(bbox.q.x);
    clip_box.q.y = float2fixed(bbox.q.y);
    code = gx_clip_to_rectangle(pgs, &clip_box);
    if (code < 0) {
        gs_grestore(pgs);
        return code;
    }

    {
        pattern_accum_param_s param;
        param.pinst = (void *)pinst;
        param.graphics_state = (void *)pgs;
        param.pinst_id = pinst->id;

        code = dev_proc(pgs->device, dev_spec_op)(pgs->device,
                                gxdso_pattern_start_accum, &param, sizeof(pattern_accum_param_s));
    }

    if (code < 0) {
        gs_grestore(pgs);
        return code;
    }

    if (ppmap->pcspace != 0)
        code = image_PaintProc(&pdc->ccolor, pgs);
    else {
        pcs = gs_cspace_new_DeviceGray(pgs->memory);
        if (pcs == NULL)
            return_error(gs_error_VMerror);
        gs_setcolorspace(pgs, pcs);
        code = mask_PaintProc(&pdc->ccolor, pgs);
    }
    if (code < 0)
        return code;

    code = gs_grestore(pgs);
    if (code < 0)
        return code;

    {
        pattern_accum_param_s param;
        param.pinst = (void *)pinst;
        param.graphics_state = (void *)pgs;
        param.pinst_id = pinst->id;

        code = dev_proc(pgs->device, dev_spec_op)(pgs->device,
                          gxdso_pattern_finish_accum, &param, sizeof(pattern_accum_param_s));
    }

    return code;
}
