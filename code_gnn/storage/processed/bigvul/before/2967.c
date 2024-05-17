pdf14_pop_transparency_state(gx_device *dev, gs_gstate *pgs)
{
     
    pdf14_device *pdev = (pdf14_device *)dev;
    pdf14_ctx *ctx = pdev->ctx;
    pdf14_mask_t *old_mask;

    if_debug0m('v', ctx->memory, "pdf14_pop_transparency_state\n");
     
    if (ctx->mask_stack != NULL) {
        old_mask = ctx->mask_stack;
        ctx->mask_stack = ctx->mask_stack->previous;
        if (old_mask->rc_mask) {
            rc_decrement(old_mask->rc_mask, "pdf14_pop_transparency_state");
        }
        gs_free_object(old_mask->memory, old_mask, "pdf14_pop_transparency_state");
         
        if (ctx->smask_depth > 0) {
            if (ctx->stack != NULL && ctx->stack->mask_stack != NULL) {
                ctx->stack->mask_stack = ctx->mask_stack;
            }
        }
    }
#ifdef DEBUG
    pdf14_debug_mask_stack_state(pdev->ctx);
#endif
    return 0;
}
