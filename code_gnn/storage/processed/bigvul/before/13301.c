pdf14_create_clist_device(gs_memory_t *mem, gs_gstate * pgs,
                                gx_device ** ppdev, gx_device * target,
                                const gs_pdf14trans_t * pdf14pct)
{
    pdf14_clist_device * dev_proto;
    pdf14_clist_device * pdev, temp_dev_proto;
    int code;
    bool has_tags = target->graphics_type_tag & GS_DEVICE_ENCODES_TAGS;
    cmm_profile_t *target_profile;
    gsicc_rendering_param_t render_cond;
    cmm_dev_profile_t *dev_profile;
    uchar k;

    code = dev_proc(target, get_profile)(target,  &dev_profile);
    if (code < 0)
        return code;
    gsicc_extract_profile(GS_UNKNOWN_TAG, dev_profile, &target_profile,
                          &render_cond);
    if_debug0m('v', pgs->memory, "[v]pdf14_create_clist_device\n");
    code = get_pdf14_clist_device_proto(target, &dev_proto,
                                 &temp_dev_proto, pgs, pdf14pct, false);
    if (code < 0)
        return code;
    code = gs_copydevice((gx_device **) &pdev,
                         (const gx_device *) dev_proto, mem);
    if (code < 0)
        return code;
     
    if (pdev->color_info.num_components > target->color_info.num_components)
        pdev->color_info.num_components = target->color_info.num_components;
    if (pdev->color_info.max_components > target->color_info.max_components)
        pdev->color_info.max_components = target->color_info.max_components;
    pdev->color_info.depth = pdev->color_info.num_components * 8;
    pdev->pad = target->pad;
    pdev->log2_align_mod = target->log2_align_mod;
    pdev->is_planar = target->is_planar;
     
    if (has_tags) {
        pdev->procs.encode_color = pdf14_encode_color_tag;
        pdev->color_info.depth += 8;
    }
    check_device_separable((gx_device *)pdev);
    gx_device_fill_in_procs((gx_device *)pdev);
    gs_pdf14_device_copy_params((gx_device *)pdev, target);
    gx_device_set_target((gx_device_forward *)pdev, target);
     
    for (k = 0; k < pdev->color_info.num_components; k++) {
        pdev->color_info.comp_bits[k] = 8;
        pdev->color_info.comp_shift[k] = (pdev->color_info.num_components - 1 - k) * 8;
    }
    code = dev_proc((gx_device *) pdev, open_device) ((gx_device *) pdev);
    pdev->pclist_device = target;
     
    if ((target_profile->data_cs == gsCIELAB || target_profile->islab) &&
        !pdev->using_blend_cs) {
        rc_assign(pdev->icc_struct->device_profile[0],
                  pgs->icc_manager->default_rgb, "pdf14_create_clist_device");
    }
    pdev->my_encode_color = pdev->procs.encode_color;
    pdev->my_decode_color = pdev->procs.decode_color;
    pdev->my_get_color_mapping_procs = pdev->procs.get_color_mapping_procs;
    pdev->my_get_color_comp_index = pdev->procs.get_color_comp_index;
    pdev->color_info.separable_and_linear =
        target->color_info.separable_and_linear;
    *ppdev = (gx_device *) pdev;
    return code;
}
