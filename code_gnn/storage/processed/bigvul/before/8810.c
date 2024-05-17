static inline bool should_invert_viewport(struct vrend_context *ctx)
{
    
   return !(ctx->sub->viewport_is_negative ^ ctx->sub->inverted_fbo_content);
}
