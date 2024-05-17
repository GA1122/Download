pdf14_pop_transparency_mask(pdf14_ctx *ctx, gs_gstate *pgs, gx_device *dev)
{
    pdf14_buf *tos = ctx->stack;
    byte *new_data_buf;
    int icc_match;
    cmm_profile_t *des_profile = tos->parent_color_info_procs->icc_profile;  
    cmm_profile_t *src_profile;
    gsicc_rendering_param_t rendering_params;
    gsicc_link_t *icc_link;
    gsicc_rendering_param_t render_cond;
    cmm_dev_profile_t *dev_profile;

    dev_proc(dev, get_profile)(dev,  &dev_profile);
    gsicc_extract_profile(GS_UNKNOWN_TAG, dev_profile, &src_profile,
                          &render_cond);
    ctx->smask_depth -= 1;
     
    if ( des_profile != NULL && src_profile != NULL ) {
        icc_match = (des_profile->hashcode ==  src_profile->hashcode);
    } else {
        icc_match = -1;
    }
    if_debug1m('v', ctx->memory, "[v]pdf14_pop_transparency_mask, idle=%d\n",
               tos->idle);
    ctx->stack = tos->saved;
    tos->saved = NULL;   
    if (tos->mask_stack) {
         
        rc_decrement(tos->mask_stack->rc_mask,
                     "pdf14_pop_transparency_mask(tos->mask_stack->rc_mask)");
        if (tos->mask_stack->rc_mask) {
            if (tos->mask_stack->rc_mask->rc.ref_count == 1){
                rc_decrement(tos->mask_stack->rc_mask,
                            "pdf14_pop_transparency_mask(tos->mask_stack->rc_mask)");
            }
        }
        tos->mask_stack = NULL;
    }
    if (tos->data == NULL ) {
         
        if (tos->alpha == 255) {
            pdf14_buf_free(tos);
            if (ctx->mask_stack != NULL) {
                pdf14_free_mask_stack(ctx, ctx->memory);
            }
        } else {
             
            if (ctx->mask_stack != NULL) {
                pdf14_free_mask_stack(ctx, ctx->memory);
            }
            ctx->mask_stack = pdf14_mask_element_new(ctx->memory);
            ctx->mask_stack->rc_mask = pdf14_rcmask_new(ctx->memory);
            ctx->mask_stack->rc_mask->mask_buf = tos;
        }
        ctx->smask_blend = false;   
    } else {
         
         
         
         
         
        new_data_buf = gs_alloc_bytes(ctx->memory, tos->planestride,
                                        "pdf14_pop_transparency_mask");
        if (new_data_buf == NULL)
            return_error(gs_error_VMerror);
         
        memset(new_data_buf, 0, tos->planestride);
         
        if (tos->SMask_SubType == TRANSPARENCY_MASK_Alpha) {
            ctx->smask_blend = false;   
            smask_copy(tos->rect.q.y - tos->rect.p.y,
                       tos->rect.q.x - tos->rect.p.x,
                       tos->rowstride,
                       (tos->data)+tos->planestride, new_data_buf);
#if RAW_DUMP
             
            dump_raw_buffer(tos->rect.q.y-tos->rect.p.y,
                        tos->rowstride, tos->n_planes,
                        tos->planestride, tos->rowstride,
                        "SMask_Pop_Alpha(Mask_Plane1)",tos->data);
            global_index++;
#endif
        } else {
            if ( icc_match == 1 || tos->n_chan == 2) {
#if RAW_DUMP
                 
                dump_raw_buffer(tos->rect.q.y-tos->rect.p.y,
                            tos->rowstride, tos->n_planes,
                            tos->planestride, tos->rowstride,
                            "SMask_Pop_Lum(Mask_Plane0)",tos->data);
                global_index++;
#endif
                 
                smask_blend(tos->data, tos->rect.q.x - tos->rect.p.x,
                            tos->rect.q.y - tos->rect.p.y, tos->rowstride,
                            tos->planestride);
#if RAW_DUMP
                 
                dump_raw_buffer(tos->rect.q.y-tos->rect.p.y,
                            tos->rowstride, tos->n_planes,
                            tos->planestride, tos->rowstride,
                            "SMask_Pop_Lum_Post_Blend",tos->data);
                global_index++;
#endif
                smask_copy(tos->rect.q.y - tos->rect.p.y,
                           tos->rect.q.x - tos->rect.p.x,
                           tos->rowstride, tos->data, new_data_buf);
            } else {
                if ( icc_match == -1 ) {
                     
                    smask_luminosity_mapping(tos->rect.q.y - tos->rect.p.y ,
                        tos->rect.q.x - tos->rect.p.x,tos->n_chan,
                        tos->rowstride, tos->planestride,
                        tos->data,  new_data_buf, ctx->additive, tos->SMask_SubType);
                } else {
                     
                     
                    rendering_params.black_point_comp = gsBLACKPTCOMP_OFF;
                    rendering_params.graphics_type_tag = GS_IMAGE_TAG;
                    rendering_params.override_icc = false;
                    rendering_params.preserve_black = gsBKPRESNOTSPECIFIED;
                    rendering_params.rendering_intent = gsPERCEPTUAL;
                    rendering_params.cmm = gsCMM_DEFAULT;
                    icc_link = gsicc_get_link_profile(pgs, dev, des_profile,
                        src_profile, &rendering_params, pgs->memory, false);
                    smask_icc(dev, tos->rect.q.y - tos->rect.p.y,
                              tos->rect.q.x - tos->rect.p.x,tos->n_chan,
                              tos->rowstride, tos->planestride,
                              tos->data, new_data_buf, icc_link);
                     
                    gsicc_release_link(icc_link);
                }
            }
        }
         
        gs_free_object(ctx->memory, tos->data, "pdf14_pop_transparency_mask");
        tos->data = new_data_buf;
         
        tos->n_chan = 1;
        tos->n_planes = 1;
         
        if (ctx->mask_stack != NULL) {
             
            pdf14_free_mask_stack(ctx, ctx->memory);
        }
        ctx->mask_stack = pdf14_mask_element_new(ctx->memory);
        if (ctx->mask_stack == NULL)
            return gs_note_error(gs_error_VMerror);
        ctx->mask_stack->rc_mask = pdf14_rcmask_new(ctx->memory);
        if (ctx->mask_stack->rc_mask == NULL)
            return gs_note_error(gs_error_VMerror);
        ctx->mask_stack->rc_mask->mask_buf = tos;
    }
    return 0;
}
