pdf14_update_device_color_procs(gx_device *dev,
                              gs_transparency_color_t group_color,
                              int64_t icc_hashcode, gs_gstate *pgs,
                              cmm_profile_t *iccprofile, bool is_mask)
{
    pdf14_device *pdevproto = NULL;
    pdf14_device *pdev = (pdf14_device *)dev;
    const pdf14_procs_t *new_14procs = NULL;
    pdf14_parent_color_t *parent_color_info;
    gx_color_polarity_t new_polarity;
    uchar new_num_comps;
    bool new_additive;
    gx_device_clist_reader *pcrdev;
    byte comp_bits[GX_DEVICE_COLOR_MAX_COMPONENTS];
    byte comp_shift[GX_DEVICE_COLOR_MAX_COMPONENTS];
    int k;
    bool has_tags = dev->graphics_type_tag & GS_DEVICE_ENCODES_TAGS;
    gsicc_rendering_param_t render_cond;
    cmm_dev_profile_t *dev_profile;

    if (pdev->ctx->stack != NULL){
        parent_color_info = pdev->ctx->stack->parent_color_info_procs;
    } else {
         
        return_error(gs_error_undefined);
    }
    if_debug0m('v', dev->memory, "[v]pdf14_update_device_color_procs\n");

    memset(comp_bits, 0, GX_DEVICE_COLOR_MAX_COMPONENTS);
    memset(comp_shift, 0, GX_DEVICE_COLOR_MAX_COMPONENTS);

     
    parent_color_info->get_cmap_procs = NULL;
    parent_color_info->parent_color_mapping_procs = NULL;
    parent_color_info->parent_color_comp_index = NULL;

    switch (group_color) {
        case GRAY_SCALE:
            new_polarity = GX_CINFO_POLARITY_ADDITIVE;
            new_num_comps = 1;
            pdevproto = (pdf14_device *)&gs_pdf14_Gray_device;
            new_additive = true;
            new_14procs = &gray_pdf14_procs;
            comp_bits[0] = 8;
            comp_shift[0] = 0;
            break;
        case DEVICE_RGB:
        case CIE_XYZ:
            new_polarity = GX_CINFO_POLARITY_ADDITIVE;
            new_num_comps = 3;
            pdevproto = (pdf14_device *)&gs_pdf14_RGB_device;
            new_additive = true;
            new_14procs = &rgb_pdf14_procs;
            for (k = 0; k < 3; k++) {
                comp_bits[k] = 8;
                comp_shift[k] = (2 - k) * 8;
            }
            break;
        case DEVICE_CMYK:
            new_polarity = GX_CINFO_POLARITY_SUBTRACTIVE;
            new_num_comps = 4;
            pdevproto = (pdf14_device *)&gs_pdf14_CMYK_device;
            new_additive = false;
             
            if (dev->color_info.num_components > 4){
                new_14procs = &cmykspot_pdf14_procs;
            } else {
                new_14procs = &cmyk_pdf14_procs;
            }
            for (k = 0; k < 4; k++) {
                comp_bits[k] = 8;
                comp_shift[k] = (3 - k) * 8;
            }
            break;
        case ICC:
             
            if (iccprofile == NULL && pdev->pclist_device != NULL) {
                 
                pcrdev = (gx_device_clist_reader *)(pdev->pclist_device);
                iccprofile = gsicc_read_serial_icc((gx_device *) pcrdev,
                                                    icc_hashcode);
                if (iccprofile == NULL)
                    return gs_throw(gs_error_unknownerror, "ICC data not found in clist");
                 
                iccprofile->dev = (gx_device *) pcrdev;
            } else {
                 
                if (iccprofile == NULL)
                    return gs_throw(gs_error_unknownerror, "ICC data unknown");
                rc_increment(iccprofile);
            }
            new_num_comps = iccprofile->num_comps;
            if (new_num_comps == 4) {
                new_additive = false;
                new_polarity = GX_CINFO_POLARITY_SUBTRACTIVE;
            } else {
                new_additive = true;
                new_polarity = GX_CINFO_POLARITY_ADDITIVE;
            }
            switch (new_num_comps) {
                case 1:
                    if (pdev->sep_device && !is_mask) {
                        pdevproto = (pdf14_device *)&gs_pdf14_Grayspot_device;
                        new_14procs = &grayspot_pdf14_procs;
                    } else {
                        pdevproto = (pdf14_device *)&gs_pdf14_Gray_device;
                        new_14procs = &gray_pdf14_procs;
                    }
                    comp_bits[0] = 8;
                    comp_shift[0] = 0;
                    break;
                case 3:
                    if (pdev->sep_device) {
                        pdevproto = (pdf14_device *)&gs_pdf14_RGBspot_device;
                        new_14procs = &rgbspot_pdf14_procs;
                    }
                    else {
                        pdevproto = (pdf14_device *)&gs_pdf14_RGB_device;
                        new_14procs = &rgb_pdf14_procs;
                    }
                    for (k = 0; k < 3; k++) {
                        comp_bits[k] = 8;
                        comp_shift[k] = (2 - k) * 8;
                    }
                    break;
                case 4:
                    if (pdev->sep_device) {
                        pdevproto = (pdf14_device *)&gs_pdf14_CMYKspot_device;
                        new_14procs = &cmykspot_pdf14_procs;
                    } else {
                        pdevproto = (pdf14_device *)&gs_pdf14_CMYK_device;
                        new_14procs = &cmyk_pdf14_procs;
                    }
                    for (k = 0; k < 4; k++) {
                        comp_bits[k] = 8;
                        comp_shift[k] = (3 - k) * 8;
                    }
                    break;
                default:
                    return_error(gs_error_rangecheck);
                    break;
            }
            break;
        default:
            return_error(gs_error_rangecheck);
            break;
        }
    if_debug2m('v', pdev->memory,
                "[v]pdf14_update_device_color_procs,num_components_old = %d num_components_new = %d\n",
                pdev->color_info.num_components,new_num_comps);

     
    parent_color_info->get_cmap_procs = pgs->get_cmap_procs;
    parent_color_info->parent_color_mapping_procs =
        pdev->procs.get_color_mapping_procs;
    parent_color_info->parent_color_comp_index =
        pdev->procs.get_color_comp_index;
    parent_color_info->parent_blending_procs = pdev->blend_procs;
    parent_color_info->polarity =              pdev->color_info.polarity;
    parent_color_info->num_components =        pdev->color_info.num_components;
    parent_color_info->isadditive =            pdev->ctx->additive;
    parent_color_info->unpack_procs =          pdev->pdf14_procs;
    parent_color_info->depth =                 pdev->color_info.depth;
    parent_color_info->max_color =             pdev->color_info.max_color;
    parent_color_info->max_gray =              pdev->color_info.max_gray;
    parent_color_info->encode =                pdev->procs.encode_color;
    parent_color_info->decode =                pdev->procs.decode_color;
    memcpy(&(parent_color_info->comp_bits), &(pdev->color_info.comp_bits),
        GX_DEVICE_COLOR_MAX_COMPONENTS);
    memcpy(&(parent_color_info->comp_shift), &(pdev->color_info.comp_shift),
        GX_DEVICE_COLOR_MAX_COMPONENTS);

     
    if (group_color == ICC && iccprofile != NULL) {
        dev_proc(dev, get_profile)(dev, &dev_profile);
        gsicc_extract_profile(GS_UNKNOWN_TAG, dev_profile,
                                &(parent_color_info->icc_profile), &render_cond);
    }
     
     
    if (pdev->sep_device && !is_mask) {
        int num_spots = parent_color_info->num_components -
            parent_color_info->icc_profile->num_comps;

        if (num_spots > 0) {
            new_num_comps += num_spots;
            for (k = 0; k < new_num_comps; k++) {
                comp_bits[k] = 8;
                comp_shift[k] = (new_num_comps - k - 1) * 8;
            }
        }
    }

    pdev->procs.get_color_mapping_procs =
        pdevproto->static_procs->get_color_mapping_procs;
    pdev->procs.get_color_comp_index =
        pdevproto->static_procs->get_color_comp_index;
    pdev->blend_procs = pdevproto->blend_procs;
    pdev->color_info.polarity = new_polarity;
    pdev->color_info.num_components = new_num_comps;
    pdev->ctx->additive = new_additive;
    pdev->pdf14_procs = new_14procs;
    pdev->color_info.depth = new_num_comps * 8;
    if (has_tags) {
        pdev->color_info.depth += 8;
    }
    memset(&(pdev->color_info.comp_bits), 0, GX_DEVICE_COLOR_MAX_COMPONENTS);
    memset(&(pdev->color_info.comp_shift), 0, GX_DEVICE_COLOR_MAX_COMPONENTS);
    memcpy(&(pdev->color_info.comp_bits), comp_bits, 4);
    memcpy(&(pdev->color_info.comp_shift), comp_shift, 4);
    pdev->color_info.max_color = 255;
    pdev->color_info.max_gray = 255;
     
    if (group_color == ICC && iccprofile != NULL) {
         
        dev->icc_struct->device_profile[0] = iccprofile;
        rc_increment(parent_color_info->icc_profile);
    }
    return 1;   
}
