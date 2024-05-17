pdf14_copy_alpha_color(gx_device * dev, const byte * data, int data_x,
           int aa_raster, gx_bitmap_id id, int x, int y, int w, int h,
                      gx_color_index color, const gx_device_color *pdc,
                      int depth, bool devn)
{
    const byte *aa_row;
    pdf14_device *pdev = (pdf14_device *)dev;
    pdf14_buf *buf = pdev->ctx->stack;
    int i, j, k;
    byte *line, *dst_ptr;
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
    bool knockout = buf->knockout;
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
    int alpha2_aa, alpha_aa, sx;
    int alpha_aa_act;
    int xoff;
    gx_color_index mask = ((gx_color_index)1 << 8) - 1;
    int shift = 8;

    if (buf->data == NULL)
        return 0;
    aa_row = data;
    if (has_tags) {
        curr_tag = (color >> (num_comp*8)) & 0xff;
    }

    if (devn) {
        if (additive) {
            for (j = 0; j < num_comp; j++) {
                src[j] = ((pdc->colors.devn.values[j]) >> shift & mask);
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
        xoff = data_x + buf->rect.p.x - x;
        w += x - buf->rect.p.x;
        x = buf->rect.p.x;
    } else {
        xoff = data_x;
    }
    if (y < buf->rect.p.y) {
      h += y - buf->rect.p.y;
      aa_row -= (y - buf->rect.p.y) * aa_raster;
      y = buf->rect.p.y;
    }
    if (x + w > buf->rect.q.x) w = buf->rect.q.x - x;
    if (y + h > buf->rect.q.y) h = buf->rect.q.y - y;
     
    if (x < buf->dirty.p.x) buf->dirty.p.x = x;
    if (y < buf->dirty.p.y) buf->dirty.p.y = y;
    if (x + w > buf->dirty.q.x) buf->dirty.q.x = x + w;
    if (y + h > buf->dirty.q.y) buf->dirty.q.y = y + h;
    line = buf->data + (x - buf->rect.p.x) + (y - buf->rect.p.y) * rowstride;

    for (j = 0; j < h; ++j, aa_row += aa_raster) {
        dst_ptr = line;
        sx = xoff;
        for (i = 0; i < w; ++i, ++sx) {
             
            if (additive) {
                for (k = 0; k < num_chan; ++k)		 
                    dst[k] = dst_ptr[k * planestride];
            } else {  
                for (k = 0; k < num_comp; ++k)
                    dst[k] = 255 - dst_ptr[k * planestride];
                dst[num_comp] = dst_ptr[num_comp * planestride];	 
            }
             
            switch(depth)
            {
            case 2:   
                alpha_aa = ((aa_row[sx >> 2] >> ((3 - (sx & 3)) << 1)) & 3) * 85;
                break;
            case 4:
                alpha2_aa = aa_row[sx >> 1];
                alpha_aa = (sx & 1 ? alpha2_aa & 0xf : alpha2_aa >> 4) * 17;
                break;
            case 8:
                alpha_aa = aa_row[sx];
                break;
            default:
                return_error(gs_error_rangecheck);
            }
            if (alpha_aa != 0) {   
                if (alpha_aa != 255) {
                     
                    alpha_aa_act = alpha_aa;
                    if (src_alpha != 255) {
                         
                        int tmp = src_alpha * alpha_aa_act + 0x80;
                        alpha_aa_act = (tmp + (tmp >> 8)) >> 8;
                    }
                     
                    src[num_comp] = alpha_aa_act;
                } else {
                     
                    src[num_comp] = src_alpha;
                }
                if (knockout) {
                    if (buf->isolated) {
                        art_pdf_knockoutisolated_group_8(dst, src, num_comp);
                    } else {
                        art_pdf_composite_knockout_8(dst, src, num_comp,
                            blend_mode, pdev->blend_procs, pdev);
                    }
                } else {
                    art_pdf_composite_pixel_alpha_8(dst, src, num_comp, blend_mode, num_comp,
                                                    pdev->blend_procs, pdev);
                }
                 
                if (additive) {
                    for (k = 0; k < num_chan; ++k)
                        dst_ptr[k * planestride] = dst[k];
                } else {
                    if (overprint && dst_ptr[num_comp * planestride] != 0) {
                        for (k = 0, comps = drawn_comps; comps != 0;
                                ++k, comps >>= 1) {
                            if ((comps & 0x1) != 0) {
                                dst_ptr[k * planestride] = 255 - dst[k];
                            }
                        }
                         
                        dst_ptr[num_comp * planestride] = dst[num_comp];
                    } else {
                        for (k = 0; k < num_comp; ++k)
                            dst_ptr[k * planestride] = 255 - dst[k];
                         
                        dst_ptr[num_comp * planestride] = dst[num_comp];
                    }
                }
                if (has_alpha_g) {
                    int tmp = (255 - dst_ptr[alpha_g_off]) * (255 - src[num_comp]) + 0x80;
                    dst_ptr[alpha_g_off] = 255 - ((tmp + (tmp >> 8)) >> 8);
                }
                if (has_shape) {
                    int tmp = (255 - dst_ptr[shape_off]) * (255 - shape) + 0x80;
                    dst_ptr[shape_off] = 255 - ((tmp + (tmp >> 8)) >> 8);
                }
                if (has_tags) {
                     
                     
                    if (src[num_comp] == 255 && blend_mode == BLEND_MODE_Normal) {
                        dst_ptr[tag_off] = curr_tag;
                    } else {
                        dst_ptr[tag_off] |= curr_tag;
                    }
                }
            }
            ++dst_ptr;
        }
        line += rowstride;
    }
    return 0;
}