pdf14_mark_fill_rectangle(gx_device * dev, int x, int y, int w, int h,
                          gx_color_index color, const gx_device_color *pdc,
                          bool devn)
{
    pdf14_device *pdev = (pdf14_device *)dev;
    pdf14_buf *buf = pdev->ctx->stack;
    int i, j, k;
    byte *dst_ptr;
    byte src[PDF14_MAX_PLANES];
    byte dst[PDF14_MAX_PLANES] = { 0 };
    gs_blend_mode_t blend_mode = pdev->blend_mode;
    bool additive = pdev->ctx->additive;
    int rowstride = buf->rowstride;
    int planestride = buf->planestride;
    gs_graphics_type_tag_t curr_tag = GS_UNKNOWN_TAG;  
    bool has_alpha_g = buf->has_alpha_g;
    bool has_shape = buf->has_shape;
    bool has_tags = buf->has_tags;
    int num_chan = buf->n_chan;
    int num_comp = num_chan - 1;
    int shape_off = num_chan * planestride;
    int alpha_g_off = shape_off + (has_shape ? planestride : 0);
    int tag_off = alpha_g_off + (has_alpha_g ? planestride : 0);
    bool overprint = pdev->overprint;
    gx_color_index drawn_comps = pdev->drawn_comps;
    gx_color_index comps;
    byte shape = 0;  
    byte src_alpha;
    const gx_color_index mask = ((gx_color_index)1 << 8) - 1;
    const int shift = 8;
    int num_spots = buf->num_spots;

    if (buf->data == NULL)
        return 0;
     
#if 0
    if (sizeof(color) <= sizeof(ulong))
        if_debug8m('v', dev->memory,
                   "[v]pdf14_mark_fill_rectangle, (%d, %d), %d x %d color = %lx  bm %d, nc %d, overprint %d\n",
                   x, y, w, h, (ulong)color, blend_mode, num_chan, overprint);
    else
        if_debug9m('v', dev->memory,
                   "[v]pdf14_mark_fill_rectangle, (%d, %d), %d x %d color = %08lx%08lx  bm %d, nc %d, overprint %d\n",
                   x, y, w, h,
                   (ulong)(color >> 8*(sizeof(color) - sizeof(ulong))), (ulong)color,
                   blend_mode, num_chan, overprint);
#endif
     
    if (has_tags) {
        curr_tag = (color >> (num_comp*8)) & 0xff;
    }
    if (devn) {
        if (additive) {
            for (j = 0; j < (num_comp - num_spots); j++) {
                src[j] = ((pdc->colors.devn.values[j]) >> shift & mask);
            }
            for (j = 0; j < num_spots; j++) {
                src[j + num_comp - num_spots] =
                    255 - ((pdc->colors.devn.values[j + num_comp - num_spots]) >> shift & mask);
            }
        } else {
            for (j = 0; j < num_comp; j++) {
                src[j] = 255 - ((pdc->colors.devn.values[j]) >> shift & mask);
            }
        }
    } else
        pdev->pdf14_procs->unpack_color(num_comp, color, pdev, src);
    src_alpha = src[num_comp] = (byte)floor (255 * pdev->alpha + 0.5);
    if (has_shape)
        shape = (byte)floor (255 * pdev->shape + 0.5);
     
    if (x < buf->rect.p.x) {
        w += x - buf->rect.p.x;
        x = buf->rect.p.x;
    }
    if (y < buf->rect.p.y) {
      h += y - buf->rect.p.y;
      y = buf->rect.p.y;
    }
    if (x + w > buf->rect.q.x) w = buf->rect.q.x - x;
    if (y + h > buf->rect.q.y) h = buf->rect.q.y - y;
     
    if (x < buf->dirty.p.x) buf->dirty.p.x = x;
    if (y < buf->dirty.p.y) buf->dirty.p.y = y;
    if (x + w > buf->dirty.q.x) buf->dirty.q.x = x + w;
    if (y + h > buf->dirty.q.y) buf->dirty.q.y = y + h;
    dst_ptr = buf->data + (x - buf->rect.p.x) + (y - buf->rect.p.y) * rowstride;
    src_alpha = 255-src_alpha;
    shape = 255-shape;
    if (!has_alpha_g)
        alpha_g_off = 0;
    if (!has_shape)
        shape_off = 0;
    if (!has_tags)
        tag_off = 0;
    rowstride -= w;
     
    if (num_comp == 1 && additive && num_spots == 0) {
        for (j = h; j > 0; --j) {
            for (i = w; i > 0; --i) {
                if (src[1] == 0) {
                     
                } else if (dst_ptr[planestride] == 0) {
                    dst_ptr[0] = src[0];
                    dst_ptr[planestride] = src[1];
                } else {
                    art_pdf_composite_pixel_alpha_8_fast_mono(dst_ptr, src,
                                                blend_mode, pdev->blend_procs,
                                                planestride, pdev);
                }
                if (alpha_g_off) {
                    int tmp = (255 - dst_ptr[alpha_g_off]) * src_alpha + 0x80;
                    dst_ptr[alpha_g_off] = 255 - ((tmp + (tmp >> 8)) >> 8);
                }
                if (shape_off) {
                    int tmp = (255 - dst_ptr[shape_off]) * shape + 0x80;
                    dst_ptr[shape_off] = 255 - ((tmp + (tmp >> 8)) >> 8);
                }
                if (tag_off) {
                     
                    if (dst_ptr[planestride] == 255) {
                        dst_ptr[tag_off] = curr_tag;
                    } else {
                        dst_ptr[tag_off] = ( dst_ptr[tag_off] |curr_tag ) & ~GS_UNTOUCHED_TAG;
                    }
                }
                ++dst_ptr;
            }
            dst_ptr += rowstride;
        }
    } else {
        for (j = h; j > 0; --j) {
            for (i = w; i > 0; --i) {
                 
                if (src[num_comp] != 0) {
                    if (dst_ptr[num_comp * planestride] == 0) {
                         
                        if (additive) {
                             
                            for (k = 0; k < (num_comp - num_spots); k++) {
                                dst_ptr[k * planestride] = src[k];
                            }
                            for (k = 0; k < num_spots; k++) {
                                dst_ptr[(k + num_comp - num_spots) * planestride] =
                                    255 - src[k + num_comp - num_spots];
                            }
                        } else {
                             
                            for (k = 0; k < num_comp; k++) {
                                dst_ptr[k * planestride] = 255 - src[k];
                            }
                        }
                         
                        dst_ptr[num_comp * planestride] = src[num_comp];
                    } else if (additive && num_spots == 0) {
                         
                        art_pdf_composite_pixel_alpha_8_fast(dst_ptr, src,
                            num_comp, blend_mode, pdev->blend_procs, planestride,
                            pdev);
                    } else {
                         
                        if (!additive) {
                             
                            for (k = 0; k < num_comp; ++k)
                                dst[k] = 255 - dst_ptr[k * planestride];
                        } else {
                             
                            for (k = 0; k < (num_comp - num_spots); k++) {
                                dst[k] = dst_ptr[k * planestride];
                            }
                            for (k = 0; k < num_spots; k++) {
                                dst[k + num_comp - num_spots] =
                                    255 - dst_ptr[(k + num_comp - num_spots) * planestride];
                            }
                        }
                        dst[num_comp] = dst_ptr[num_comp * planestride];
                         
                        if (num_spots > 0 && !blend_valid_for_spot(blend_mode))
                        {
                             
                            byte temp_spot_src = src[num_comp - num_spots];
                            byte temp_spot_dst = dst[num_comp - num_spots];
                            src[num_comp - num_spots] = src[num_comp];
                            dst[num_comp - num_spots] = dst[num_comp];

                             
                            art_pdf_composite_pixel_alpha_8(dst, src,
                                num_comp - num_spots, blend_mode,
                                pdev->blend_procs, pdev);

                             
                            dst[num_comp - num_spots] = temp_spot_dst;
                            src[num_comp - num_spots] = temp_spot_src;
                            art_pdf_composite_pixel_alpha_8(&(dst[num_comp - num_spots]),
                                &(src[num_comp - num_spots]), num_spots,
                                BLEND_MODE_Normal, pdev->blend_procs, pdev);
                        } else {
                            art_pdf_composite_pixel_alpha_8(dst, src, num_comp, blend_mode,
                                pdev->blend_procs, pdev);
                        }
                         
                        if (!additive && overprint) {
                            for (k = 0, comps = drawn_comps; comps != 0; ++k, comps >>= 1) {
                                if ((comps & 0x1) != 0) {
                                    dst_ptr[k * planestride] = 255 - dst[k];
                                }
                            }
                        } else {
                             
                            if (!additive) {
                                 
                                for (k = 0; k < num_comp; ++k)
                                    dst_ptr[k * planestride] = 255 - dst[k];

                            } else {
                                 
                                for (k = 0; k < (num_comp - num_spots); k++) {
                                    dst_ptr[k * planestride] = dst[k];
                                }
                                for (k = 0; k < num_spots; k++) {
                                    dst_ptr[(k + num_comp - num_spots) * planestride] =
                                        255 - dst[k + num_comp - num_spots];
                                }
                            }
                        }
                         
                        dst_ptr[num_comp * planestride] = dst[num_comp];
                    }
                }
                if (alpha_g_off) {
                    int tmp = (255 - dst_ptr[alpha_g_off]) * src_alpha + 0x80;
                    dst_ptr[alpha_g_off] = 255 - ((tmp + (tmp >> 8)) >> 8);
                }
                if (shape_off) {
                    int tmp = (255 - dst_ptr[shape_off]) * shape + 0x80;
                    dst_ptr[shape_off] = 255 - ((tmp + (tmp >> 8)) >> 8);
                }
                if (tag_off) {
                     
                    if (dst[num_comp] == 255) {
                        dst_ptr[tag_off] = curr_tag;
                    } else {
                        dst_ptr[tag_off] = ( dst_ptr[tag_off] |curr_tag ) & ~GS_UNTOUCHED_TAG;
                    }
                }
                ++dst_ptr;
            }
            dst_ptr += rowstride;
        }
    }

#if 0
 
     

        if(global_index/10.0 == (int) (global_index/10.0) )
                dump_raw_buffer(pdev->ctx->stack->rect.q.y-pdev->ctx->stack->rect.p.y,
                                        pdev->ctx->stack->rect.q.x-pdev->ctx->stack->rect.p.x,
                                        pdev->ctx->stack->n_planes,
                                        pdev->ctx->stack->planestride, pdev->ctx->stack->rowstride,
                                        "Draw_Rect",pdev->ctx->stack->data);

    global_index++;
#endif
    return 0;
}
