pdf14_pop_transparency_group(gs_gstate *pgs, pdf14_ctx *ctx,
    const pdf14_nonseparable_blending_procs_t * pblend_procs,
    int tos_num_color_comp, cmm_profile_t *curr_icc_profile, gx_device *dev)
{
    pdf14_buf *tos = ctx->stack;
    pdf14_buf *nos = tos->saved;
    pdf14_mask_t *mask_stack = tos->mask_stack;
    pdf14_buf *maskbuf;
    int x0, x1, y0, y1;
    int nos_num_color_comp;
    bool icc_match;
    pdf14_device *pdev = (pdf14_device *)dev;
    bool overprint = pdev->overprint;
    gx_color_index drawn_comps = pdev->drawn_comps;

    if (nos == NULL)
        return_error(gs_error_unknownerror);   

    nos_num_color_comp = nos->parent_color_info_procs->num_components - nos->num_spots;
    tos_num_color_comp = tos_num_color_comp - tos->num_spots;

#ifdef DEBUG
    pdf14_debug_mask_stack_state(ctx);
#endif
    if (mask_stack == NULL) {
        maskbuf = NULL;
    } else {
        maskbuf = mask_stack->rc_mask->mask_buf;
    }
     
    rect_intersect(tos->dirty, tos->rect);
    rect_intersect(nos->dirty, nos->rect);
     
     
    y0 = max(tos->dirty.p.y, nos->rect.p.y);
    y1 = min(tos->dirty.q.y, nos->rect.q.y);
    x0 = max(tos->dirty.p.x, nos->rect.p.x);
    x1 = min(tos->dirty.q.x, nos->rect.q.x);
    if (ctx->mask_stack) {
         
        rc_decrement(ctx->mask_stack->rc_mask, "pdf14_pop_transparency_group");
        if (ctx->mask_stack->rc_mask == NULL ){
            gs_free_object(ctx->memory, ctx->mask_stack, "pdf14_pop_transparency_group");
        }
        ctx->mask_stack = NULL;
    }
    ctx->mask_stack = mask_stack;   
    tos->mask_stack = NULL;         
    if (tos->idle)
        goto exit;
    if (maskbuf != NULL && maskbuf->data == NULL && maskbuf->alpha == 255)
        goto exit;

#if RAW_DUMP
     
    dump_raw_buffer(ctx->stack->rect.q.y-ctx->stack->rect.p.y,
                ctx->stack->rowstride, ctx->stack->n_planes,
                ctx->stack->planestride, ctx->stack->rowstride,
                "aaTrans_Group_Pop",ctx->stack->data);
#endif
 
    if (nos->parent_color_info_procs->icc_profile != NULL) {
        icc_match = (nos->parent_color_info_procs->icc_profile->hashcode !=
                        curr_icc_profile->hashcode);
    } else {
         
        icc_match = false;
    }
     
    if ((nos->parent_color_info_procs->parent_color_mapping_procs != NULL &&
        nos_num_color_comp != tos_num_color_comp) || icc_match) {
        if (x0 < x1 && y0 < y1) {
            pdf14_buf *result;
            bool did_alloc;  

            result = pdf14_transform_color_buffer(pgs, ctx, dev, tos, tos->data,
                curr_icc_profile, nos->parent_color_info_procs->icc_profile,
                tos->rect.p.x, tos->rect.p.y, tos->rect.q.x - tos->rect.p.x,
                tos->rect.q.y - tos->rect.p.y, &did_alloc);
            if (result == NULL)
                return_error(gs_error_unknownerror);   

#if RAW_DUMP
             
            dump_raw_buffer(ctx->stack->rect.q.y-ctx->stack->rect.p.y,
                            ctx->stack->rowstride, ctx->stack->n_chan,
                            ctx->stack->planestride, ctx->stack->rowstride,
                            "aCMTrans_Group_ColorConv",ctx->stack->data);
#endif
              
            pdf14_compose_group(tos, nos, maskbuf, x0, x1, y0, y1, nos->n_chan,
                 nos->parent_color_info_procs->isadditive,
                 nos->parent_color_info_procs->parent_blending_procs,
                 false, drawn_comps, ctx->memory, dev);
        }
    } else {
         
        if (x0 < x1 && y0 < y1)
            pdf14_compose_group(tos, nos, maskbuf, x0, x1, y0, y1, nos->n_chan,
                                ctx->additive, pblend_procs, overprint,
                                drawn_comps, ctx->memory, dev);
    }
exit:
    ctx->stack = nos;
     
    if (ctx->smask_depth > 0 && maskbuf != NULL) {
         
        ctx->smask_blend = true;
    }
    if_debug1m('v', ctx->memory, "[v]pop buf, idle=%d\n", tos->idle);
    pdf14_buf_free(tos);
    return 0;
}
