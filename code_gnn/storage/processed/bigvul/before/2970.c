pdf14_push_transparency_mask(pdf14_ctx *ctx, gs_int_rect *rect,	byte bg_alpha,
                             byte *transfer_fn, bool idle, bool replacing,
                             uint mask_id, gs_transparency_mask_subtype_t subtype,
                             int numcomps, int Background_components,
                             const float Background[], int Matte_components,
                             const float Matte[], const float GrayBackground)
{
    pdf14_buf *buf;
    unsigned char *curr_ptr, gray;

    if_debug2m('v', ctx->memory,
               "[v]pdf14_push_transparency_mask, idle=%d, replacing=%d\n",
               idle, replacing);
    ctx->smask_depth += 1;

     
     
    buf = pdf14_buf_new(rect, false, false, false, idle, numcomps + 1, 0,
                        ctx->memory);
    if (buf == NULL)
        return_error(gs_error_VMerror);
    buf->alpha = bg_alpha;
     
    buf->isolated = true;
    buf->knockout = false;
    buf->shape = 0xff;
    buf->blend_mode = BLEND_MODE_Normal;
    buf->transfer_fn = transfer_fn;
    buf->matte_num_comps = Matte_components;
    if (Matte_components) {
        buf->matte = (byte *)gs_alloc_bytes(ctx->memory, sizeof(float)*Matte_components,
                                            "pdf14_push_transparency_mask");
        if (buf->matte == NULL)
            return_error(gs_error_VMerror);
        memcpy(buf->matte, Matte, size_of(float)*Matte_components);
    }
    buf->mask_id = mask_id;
     
    buf->mask_stack = ctx->mask_stack;
    if (buf->mask_stack){
        rc_increment(buf->mask_stack->rc_mask);
    }
#if RAW_DUMP
     
    if (ctx->stack->planestride > 0 ){
        dump_raw_buffer(ctx->stack->rect.q.y-ctx->stack->rect.p.y,
                    ctx->stack->rowstride, ctx->stack->n_planes,
                    ctx->stack->planestride, ctx->stack->rowstride,
                    "Raw_Buf_PreSmask",ctx->stack->data);
        global_index++;
    }
#endif
    buf->saved = ctx->stack;
    ctx->stack = buf;
     
    buf->SMask_SubType = subtype;
    if (buf->data != NULL){
         
         
         
        if ( Background_components && GrayBackground != 0.0 ) {
            curr_ptr = buf->data;
            gray = (unsigned char) (255.0 * GrayBackground);
            memset(curr_ptr, gray, buf->planestride);
                curr_ptr +=  buf->planestride;
             
            memset(curr_ptr, 255, buf->planestride *(buf->n_chan - 1));
        } else {
             
            memset(buf->data, 0, buf->planestride * buf->n_chan);
        }
    }
    return 0;
}
