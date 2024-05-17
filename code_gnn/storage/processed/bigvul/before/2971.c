pdf14_push_transparency_state(gx_device *dev, gs_gstate *pgs)
{
     

    pdf14_device *pdev = (pdf14_device *)dev;
    pdf14_ctx *ctx = pdev->ctx;
    pdf14_mask_t *new_mask;

    if_debug0m('v', ctx->memory, "pdf14_push_transparency_state\n");
     
     
    if ( ctx->mask_stack != NULL ) {
        new_mask = pdf14_mask_element_new(ctx->memory);
         
        new_mask->rc_mask = ctx->mask_stack->rc_mask;
        rc_increment(new_mask->rc_mask);
        new_mask->previous = ctx->mask_stack;
        ctx->mask_stack = new_mask;
    }
#ifdef DEBUG
    pdf14_debug_mask_stack_state(pdev->ctx);
#endif
    return(0);
}
