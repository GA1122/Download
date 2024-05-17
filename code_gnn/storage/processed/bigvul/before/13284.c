pdf14_clist_create_compositor(gx_device	* dev, gx_device ** pcdev,
    const gs_composite_t * pct, gs_gstate * pgs, gs_memory_t * mem,
    gx_device *cdev)
{
    pdf14_clist_device * pdev = (pdf14_clist_device *)dev;
    int code, is_pdf14_compositor;
    const gs_pdf14trans_t * pdf14pct = (const gs_pdf14trans_t *) pct;

     
    if ((is_pdf14_compositor = gs_is_pdf14trans_compositor(pct)) != 0) {
        switch (pdf14pct->params.pdf14_op) {
            case PDF14_PUSH_DEVICE:
                 
                pdev->saved_target_color_info = pdev->target->color_info;
                pdev->target->color_info = pdev->color_info;
                pdev->saved_target_encode_color = pdev->target->procs.encode_color;
                pdev->saved_target_decode_color = pdev->target->procs.decode_color;
                pdev->target->procs.encode_color = pdev->procs.encode_color =
                                                   pdev->my_encode_color;
                pdev->target->procs.decode_color = pdev->procs.decode_color =
                                                   pdev->my_decode_color;
                pdev->saved_target_get_color_mapping_procs =
                                    pdev->target->procs.get_color_mapping_procs;
                pdev->saved_target_get_color_comp_index =
                                        pdev->target->procs.get_color_comp_index;
                pdev->target->procs.get_color_mapping_procs =
                        pdev->procs.get_color_mapping_procs =
                        pdev->my_get_color_mapping_procs;
                pdev->target->procs.get_color_comp_index =
                        pdev->procs.get_color_comp_index =
                        pdev->my_get_color_comp_index;
                pdev->save_get_cmap_procs = pgs->get_cmap_procs;
                pgs->get_cmap_procs = pdf14_get_cmap_procs;
                gx_set_cmap_procs(pgs, dev);
                code = pdf14_recreate_clist_device(mem, pgs, dev, pdf14pct);
                pdev->blend_mode = pdev->text_knockout = 0;
                pdev->opacity = pdev->shape = 0.0;
                if (code < 0)
                    return code;
                 
                {
                    gs_composite_t pctemp = *pct;

                    pctemp.type = &gs_composite_pdf14trans_no_clist_writer_type;
                    code = dev_proc(pdev->target, create_compositor)
                                (pdev->target, pcdev, &pctemp, pgs, mem, cdev);
                    *pcdev = dev;
                    return code;
                }
            case PDF14_POP_DEVICE:
                 
                pdev->target->color_info = pdev->saved_target_color_info;
                pdev->target->procs.encode_color =
                                        pdev->saved_target_encode_color;
                pdev->target->procs.decode_color =
                                        pdev->saved_target_decode_color;
                pdev->target->procs.get_color_mapping_procs =
                                    pdev->saved_target_get_color_mapping_procs;
                pdev->target->procs.get_color_comp_index =
                                    pdev->saved_target_get_color_comp_index;
                pgs->get_cmap_procs = pdev->save_get_cmap_procs;
                gx_set_cmap_procs(pgs, pdev->target);
                gx_device_decache_colors(pdev->target);
                 
                pdf14_disable_clist_device(mem, pgs, dev);
                 
                code = cmd_put_color_mapping(
                        (gx_device_clist_writer *)(pdev->target), pgs);
                if (code < 0)
                    return code;
                break;
            case PDF14_BEGIN_TRANS_GROUP:
                 
                code = pdf14_clist_update_params(pdev, pgs, true,
                                                 (gs_pdf14trans_params_t *)&(pdf14pct->params));
                if (code < 0)
                    return code;
                if (pdf14pct->params.Background_components != 0 &&
                    pdf14pct->params.Background_components !=
                    pdev->color_info.num_components)
                    return_error(gs_error_rangecheck);
                 
                 
                pdf14_push_parent_color(dev, pgs);

                code = pdf14_update_device_color_procs_push_c(dev,
                                pdf14pct->params.group_color,
                                pdf14pct->params.icc_hash, pgs,
                                pdf14pct->params.iccprofile, false);
                if (code < 0)
                    return code;
                break;
            case PDF14_BEGIN_TRANS_MASK:
                 
                 
                if (pdf14pct->params.subtype == TRANSPARENCY_MASK_None)
                    break;
                pdf14_push_parent_color(dev, pgs);
                 
                if (pdf14pct->params.iccprofile == NULL) {
                    gs_pdf14trans_params_t *pparams_noconst = (gs_pdf14trans_params_t *)&(pdf14pct->params);

                    pparams_noconst->iccprofile = gsicc_read_serial_icc((gx_device *) cdev,
                                                       pdf14pct->params.icc_hash);
                    if (pparams_noconst->iccprofile == NULL)
                        return gs_throw(-1, "ICC data not found in clist");
                     
                    pparams_noconst->iccprofile->dev = (gx_device *)cdev;
                     
                    if (pparams_noconst->iccprofile->buffer == NULL) {
                        gcmmhprofile_t dummy = gsicc_get_profile_handle_clist(pparams_noconst->iccprofile, mem);

                        if (dummy == NULL)
                            return_error(gs_error_VMerror);
                    }
                }
                 
                code = pdf14_update_device_color_procs_push_c(dev,
                                  pdf14pct->params.group_color,
                                  pdf14pct->params.icc_hash, pgs,
                                  pdf14pct->params.iccprofile, true);
                if (code < 0)
                    return code;
                 
                break;
            case PDF14_END_TRANS_GROUP:
            case PDF14_END_TRANS_MASK:
                 
                code = pdf14_update_device_color_procs_pop_c(dev,pgs);
                if (code < 0)
                    return code;
                break;
            case PDF14_PUSH_TRANS_STATE:
                break;
            case PDF14_POP_TRANS_STATE:
                break;
            case PDF14_PUSH_SMASK_COLOR:
                code = pdf14_increment_smask_color(pgs,dev);
                *pcdev = dev;
                return code;   
                break;
            case PDF14_POP_SMASK_COLOR:
                code = pdf14_decrement_smask_color(pgs,dev);
                *pcdev = dev;
                return code;   
                break;
            case PDF14_SET_BLEND_PARAMS:
                 
                code = pdf14_clist_update_params(pdev, pgs, false,
                                                 (gs_pdf14trans_params_t *)&(pdf14pct->params));
                *pcdev = dev;
                return code;
                break;
            case PDF14_ABORT_DEVICE:
                break;
            default:
                break;		 
        }
    }
    code = dev_proc(pdev->target, create_compositor)
                        (pdev->target, pcdev, pct, pgs, mem, cdev);
     
     
    if (is_pdf14_compositor && pdf14pct->params.pdf14_op == PDF14_POP_DEVICE &&
        pdev->target->stype == &st_pdf14_accum) {

        int y, rows_used;
        byte *linebuf = gs_alloc_bytes(mem, gx_device_raster((gx_device *)pdev, true), "pdf14-clist_accum pop dev");
        byte *actual_data;
        gx_device *tdev = pdev->target;      
         
        gx_device *target = ((pdf14_device *)((gx_device_pdf14_accum *)(tdev))->save_p14dev)->target;
        gs_image1_t image;
        gs_color_space *pcs;
        gx_image_enum_common_t *info;
        gx_image_plane_t planes;
        gsicc_rendering_param_t render_cond;
        cmm_dev_profile_t *dev_profile;

         
        code = gs_cspace_build_ICC(&pcs, NULL, pgs->memory);
        if (linebuf == NULL || pcs == NULL)
            goto put_accum_error;

         
        code = dev_proc(dev, get_profile)(dev,  &dev_profile);
        if (code < 0) {
            rc_decrement_only_cs(pcs, "pdf14_put_image");
            return code;
        }
        gsicc_extract_profile(GS_UNKNOWN_TAG, dev_profile,
                              &(pcs->cmm_icc_profile_data), &render_cond);
         
        rc_increment(pcs->cmm_icc_profile_data);
        gscms_set_icc_range(&(pcs->cmm_icc_profile_data));

        gs_image_t_init_adjust(&image, pcs, false);
        image.ImageMatrix.xx = (float)pdev->width;
        image.ImageMatrix.yy = (float)pdev->height;
        image.Width = pdev->width;
        image.Height = pdev->height;
        image.BitsPerComponent = 8;
        ctm_only_writable(pgs).xx = (float)pdev->width;
        ctm_only_writable(pgs).xy = 0;
        ctm_only_writable(pgs).yx = 0;
        ctm_only_writable(pgs).yy = (float)pdev->height;
        ctm_only_writable(pgs).tx = 0.0;
        ctm_only_writable(pgs).ty = 0.0;
        code = dev_proc(target, begin_typed_image) (target,
                                                    pgs, NULL,
                                                    (gs_image_common_t *)&image,
                                                    NULL, NULL, NULL,
                                                    pgs->memory, &info);
        if (code < 0)
            goto put_accum_error;
        for (y=0; y < tdev->height; y++) {
            code = dev_proc(tdev, get_bits)(tdev, y, linebuf, &actual_data);
            planes.data = actual_data;
            planes.data_x = 0;
            planes.raster = tdev->width * tdev->color_info.num_components;
            if ((code = info->procs->plane_data(info, &planes, 1, &rows_used)) < 0)
                goto put_accum_error;
        }
        info->procs->end_image(info, true);

put_accum_error:
        gs_free_object(pdev->memory, linebuf, "pdf14_put_image");
         
        rc_decrement_only_cs(pcs, "pdf14_put_image");
        dev_proc(tdev, close_device)(tdev);	 
         
        gx_device_set_target((gx_device_forward *)pdev,
                             ((gx_device_pdf14_accum *)(pdev->target))->save_p14dev);
        pdev->pclist_device = pdev->target;          
        *pcdev = pdev->target;			     
        pdev->color_info = target->color_info;       
        gs_free_object(tdev->memory, tdev, "popdevice pdf14-accum");
        return 0;		 
    }

    if (*pcdev != pdev->target)
        gx_device_set_target((gx_device_forward *)pdev, *pcdev);
    *pcdev = dev;
    return code;
}
