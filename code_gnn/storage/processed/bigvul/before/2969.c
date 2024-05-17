pdf14_push_transparency_group(pdf14_ctx	*ctx, gs_int_rect *rect, bool isolated,
                              bool knockout, byte alpha, byte shape,
                              gs_blend_mode_t blend_mode, bool idle, uint mask_id,
                              int numcomps, bool cm_back_drop,
                              cmm_profile_t *group_profile,
                              cmm_profile_t *tos_profile, gs_gstate *pgs,
                              gx_device *dev)
{
    pdf14_buf *tos = ctx->stack;
    pdf14_buf *buf, *backdrop;
    bool has_shape, has_tags;

    if_debug1m('v', ctx->memory,
               "[v]pdf14_push_transparency_group, idle = %d\n", idle);

     
    has_shape = tos->has_shape || tos->knockout;
     
    has_tags = tos->has_tags;

     
     
    buf = pdf14_buf_new(rect, has_tags, !isolated, has_shape, idle, numcomps + 1,
                        tos->num_spots, ctx->memory);
    if (buf == NULL)
        return_error(gs_error_VMerror);
    if_debug4m('v', ctx->memory,
        "[v]base buf: %d x %d, %d color channels, %d planes\n",
        buf->rect.q.x, buf->rect.q.y, buf->n_chan, buf->n_planes);
    buf->isolated = isolated;
    buf->knockout = knockout;
    buf->alpha = alpha;
    buf->shape = shape;
    buf->blend_mode = blend_mode;
    buf->mask_id = mask_id;
    buf->mask_stack = ctx->mask_stack;  
    ctx->mask_stack = NULL;  
    buf->saved = tos;
    ctx->stack = buf;
    if (buf->data == NULL)
        return 0;
    if (idle)
        return 0;
    backdrop = pdf14_find_backdrop_buf(ctx);
    if (backdrop == NULL) {
         
        memset(buf->data, 0, buf->planestride * (buf->n_chan +
                                                 (buf->has_shape ? 1 : 0) +
                                                 (buf->has_alpha_g ? 1 : 0)));
    } else {
        if (!buf->knockout) {
            if (!cm_back_drop) {
                pdf14_preserve_backdrop(buf, tos, false);
            } else {
                 
                pdf14_preserve_backdrop_cm(buf, group_profile, tos, tos_profile,
                                           ctx->memory, pgs, dev, false);
            }
        }
    }

     
    if (buf->knockout) {
        buf->backdrop = gs_alloc_bytes(ctx->memory, buf->planestride * buf->n_chan,
                                        "pdf14_push_transparency_group");
        if (buf->backdrop == NULL) {
            return gs_throw(gs_error_VMerror, "Knockout backdrop allocation failed");
        }
        if (buf->isolated) {
             
            memset(buf->backdrop, 0, buf->planestride * buf->n_chan);
        } else {
             
             
            pdf14_buf *check = tos;
            pdf14_buf *child = NULL;   
            cmm_profile_t *prev_knockout_profile;

            while (check != NULL) {
                if (check->isolated)
                    break;
                if (check->knockout) {
                    break;
                }
                child = check;
                check = check->saved;
            }
             
            if (check == NULL) {
                prev_knockout_profile = tos_profile;
                check = tos;
            } else {
                if (child == NULL) {
                    prev_knockout_profile = tos_profile;
                } else {
                    prev_knockout_profile  = child->parent_color_info_procs->icc_profile;
                }
            }
            if (!cm_back_drop) {
                pdf14_preserve_backdrop(buf, check, false);
            } else {
                 
                pdf14_preserve_backdrop_cm(buf, group_profile, check,
                                           prev_knockout_profile, ctx->memory, pgs,
                                           dev, false);
            }
            memcpy(buf->backdrop, buf->data, buf->planestride * buf->n_chan);
        }
#if RAW_DUMP
         
        dump_raw_buffer(ctx->stack->rect.q.y-ctx->stack->rect.p.y,
                    ctx->stack->rowstride, buf->n_chan,
                    ctx->stack->planestride, ctx->stack->rowstride,
                    "KnockoutBackDrop", buf->backdrop);
        global_index++;
#endif
    } else {
        buf->backdrop = NULL;
    }
#if RAW_DUMP
     
    dump_raw_buffer(ctx->stack->rect.q.y-ctx->stack->rect.p.y,
                ctx->stack->rowstride, ctx->stack->n_planes,
                ctx->stack->planestride, ctx->stack->rowstride,
                "TransGroupPush", ctx->stack->data);
    global_index++;
#endif
    return 0;
}
