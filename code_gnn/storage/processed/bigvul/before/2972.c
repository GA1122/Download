pdf14_put_image(gx_device * dev, gs_gstate * pgs, gx_device * target)
{
    const pdf14_device * pdev = (pdf14_device *)dev;
    int code;
    gs_image1_t image;
    gx_image_enum_common_t *info;
    pdf14_buf *buf = pdev->ctx->stack;
    gs_int_rect rect = buf->rect;
    int y;
    int num_comp = buf->n_chan - 1;
    byte *linebuf;
    gs_color_space *pcs;
    int x1, y1, width, height;
    byte *buf_ptr;
    bool data_blended = false;
    int num_rows_left;
    gsicc_rendering_param_t render_cond;
    cmm_dev_profile_t *dev_profile;
    cmm_dev_profile_t *dev_target_profile;
    byte bg = pdev->ctx->additive ? 255 : 0;

     
    if (buf->saved != NULL) {
        return gs_throw(gs_error_unknownerror, "PDF14 device push/pop out of sync");
    }

    if_debug0m('v', dev->memory, "[v]pdf14_put_image\n");
    rect_intersect(rect, buf->dirty);
    x1 = min(pdev->width, rect.q.x);
    y1 = min(pdev->height, rect.q.y);
    width = x1 - rect.p.x;
    height = y1 - rect.p.y;
#ifdef DUMP_TO_PNG
    dump_planar_rgba(pdev->memory, buf);
#endif
    if (width <= 0 || height <= 0 || buf->data == NULL)
        return 0;
    buf_ptr = buf->data + rect.p.y * buf->rowstride + rect.p.x;

     
    code = dev_proc(target, get_profile)(target,  &dev_target_profile);
    if (code < 0)
        return code;
    if (dev_target_profile == NULL)
        return gs_throw_code(gs_error_Fatal);

     
    if (dev_proc(target, put_image) != gx_default_put_image) {
        pdf14_buf *cm_result = NULL;
        int alpha_offset, tag_offset;
        const byte *buf_ptrs[GS_CLIENT_COLOR_MAX_COMPONENTS];
        int i;

         
        if (pdev->using_blend_cs) {
            cmm_profile_t *src_profile;
            cmm_profile_t *des_profile;
            bool did_alloc;

            code = dev_proc(dev, get_profile)(dev, &dev_profile);
            if (code < 0) {
                return code;
            }
            gsicc_extract_profile(GS_UNKNOWN_TAG, dev_profile, &src_profile,
                &render_cond);
            gsicc_extract_profile(GS_UNKNOWN_TAG, dev_target_profile, &des_profile,
                &render_cond);

#if RAW_DUMP
            dump_raw_buffer(height, width, buf->n_planes, buf->planestride,
                buf->rowstride, "pre_blend_cs", buf_ptr);
            global_index++;
#endif

            cm_result = pdf14_transform_color_buffer(pgs, pdev->ctx, dev, buf,
                buf_ptr, src_profile, des_profile, rect.p.x, rect.p.y, width,
                height, &did_alloc);

            if (cm_result == NULL)
                return_error(gs_error_VMerror);

             
            buf = cm_result;
            num_comp = buf->n_chan - 1;

             
            if (did_alloc)
                buf_ptr = cm_result->data;   

#if RAW_DUMP
            dump_raw_buffer(height, width, buf->n_planes, buf->planestride,
                buf->rowstride, "post_blend_cs", buf_ptr);
            global_index++;
#endif
             
            if (des_profile->num_comps == 4)
                bg = 0;
            else
                bg = 255;
        }
        alpha_offset = num_comp;
        tag_offset = buf->has_tags ? buf->n_chan : 0;

         
        for (i = 0; i < buf->n_planes; i++)
            buf_ptrs[i] = buf_ptr + i * buf->planestride;
        code = dev_proc(target, put_image) (target, buf_ptrs, num_comp,
                                            rect.p.x, rect.p.y, width, height,
                                            buf->rowstride, alpha_offset,
                                            tag_offset);
        if (code == 0) {
             
#if RAW_DUMP
            dump_raw_buffer(height, width, buf->n_planes,
                        pdev->ctx->stack->planestride, pdev->ctx->stack->rowstride,
                        "pre_final_blend",buf_ptr);
            global_index++;
#endif
            gx_blend_image_buffer(buf_ptr, width, height, buf->rowstride,
                                  buf->planestride, num_comp, bg);
#if RAW_DUMP
            dump_raw_buffer(height, width, buf->n_planes,
                        pdev->ctx->stack->planestride, pdev->ctx->stack->rowstride,
                        "post_final_blend",buf_ptr);
            global_index++;
            clist_band_count++;
#endif
            data_blended = true;

             
            alpha_offset = 0;
            code = dev_proc(target, put_image) (target, buf_ptrs, num_comp,
                                                rect.p.x, rect.p.y, width, height,
                                                buf->rowstride, alpha_offset,
                                                tag_offset);
        }
        if (code > 0) {
             
            num_rows_left = height - code;
            while (num_rows_left > 0) {
                code = dev_proc(target, put_image) (target, buf_ptrs, num_comp,
                                                    rect.p.x, rect.p.y + code, width,
                                                    num_rows_left, buf->rowstride,
                                                    alpha_offset, tag_offset);
                num_rows_left = num_rows_left - code;
            }
            return 0;
        }
    }
     
    code = gs_cspace_build_ICC(&pcs, NULL, pgs->memory);
    if (pcs == NULL)
        return_error(gs_error_VMerror);
    if (code < 0)
        return code;
     
    code = dev_proc(dev, get_profile)(dev,  &dev_profile);
    if (code < 0) {
        rc_decrement_only_cs(pcs, "pdf14_put_image");
        return code;
    }
    gsicc_extract_profile(GS_UNKNOWN_TAG, dev_profile,
                          &(pcs->cmm_icc_profile_data), &render_cond);
     
    gsicc_adjust_profile_rc(pcs->cmm_icc_profile_data, 1, "pdf14_put_image");
    gsicc_set_icc_range(&(pcs->cmm_icc_profile_data));
    gs_image_t_init_adjust(&image, pcs, false);
    image.ImageMatrix.xx = (float)width;
    image.ImageMatrix.yy = (float)height;
    image.Width = width;
    image.Height = height;
    image.BitsPerComponent = 8;
    ctm_only_writable(pgs).xx = (float)width;
    ctm_only_writable(pgs).xy = 0;
    ctm_only_writable(pgs).yx = 0;
    ctm_only_writable(pgs).yy = (float)height;
    ctm_only_writable(pgs).tx = (float)rect.p.x;
    ctm_only_writable(pgs).ty = (float)rect.p.y;
    code = dev_proc(target, begin_typed_image) (target,
                                                pgs, NULL,
                                                (gs_image_common_t *)&image,
                                                NULL, NULL, NULL,
                                                pgs->memory, &info);
    if (code < 0) {
        rc_decrement_only_cs(pcs, "pdf14_put_image");
        return code;
    }
#if RAW_DUMP
     
    dump_raw_buffer(pdev->ctx->stack->rect.q.y-pdev->ctx->stack->rect.p.y,
                pdev->ctx->stack->rect.q.x-pdev->ctx->stack->rect.p.x,
                                pdev->ctx->stack->n_planes,
                pdev->ctx->stack->planestride, pdev->ctx->stack->rowstride,
                "pdF14_putimage",pdev->ctx->stack->data);
    dump_raw_buffer(height, width, buf->n_planes,
                pdev->ctx->stack->planestride, pdev->ctx->stack->rowstride,
                "PDF14_PUTIMAGE_SMALL",buf_ptr);
    global_index++;
    if (!data_blended) {
        clist_band_count++;
    }
#endif
    linebuf = gs_alloc_bytes(pdev->memory, width * num_comp, "pdf14_put_image");
    for (y = 0; y < height; y++) {
        gx_image_plane_t planes;
        int rows_used,k,x;

        if (data_blended) {
            for (x = 0; x < width; x++) {
                for (k = 0; k < num_comp; k++) {
                    linebuf[x * num_comp + k] = buf_ptr[x + buf->planestride * k];
                }
            }
        } else {
            gx_build_blended_image_row(buf_ptr, buf->planestride, width,
                                       num_comp, bg, linebuf);
        }
        planes.data = linebuf;
        planes.data_x = 0;
        planes.raster = width * num_comp;
        info->procs->plane_data(info, &planes, 1, &rows_used);
         
        buf_ptr += buf->rowstride;
    }
    gs_free_object(pdev->memory, linebuf, "pdf14_put_image");
    info->procs->end_image(info, true);
     
    rc_decrement_only_cs(pcs, "pdf14_put_image");
    return code;
}
