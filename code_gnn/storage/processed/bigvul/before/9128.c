static int vrend_decode_set_viewport_state(struct vrend_decode_ctx *ctx, int length)
{
   struct pipe_viewport_state vps[PIPE_MAX_VIEWPORTS];
   int i, v;
   uint32_t num_viewports, start_slot;
   if (length < 1)
      return EINVAL;

   if ((length - 1) % 6)
      return EINVAL;

   num_viewports = (length - 1) / 6;
   start_slot = get_buf_entry(ctx, VIRGL_SET_VIEWPORT_START_SLOT);

   if (num_viewports > PIPE_MAX_VIEWPORTS ||
       start_slot > (PIPE_MAX_VIEWPORTS - num_viewports))
      return EINVAL;

   for (v = 0; v < num_viewports; v++) {
      for (i = 0; i < 3; i++)
         vps[v].scale[i] = uif(get_buf_entry(ctx, VIRGL_SET_VIEWPORT_STATE_SCALE_0(v) + i));
      for (i = 0; i < 3; i++)
         vps[v].translate[i] = uif(get_buf_entry(ctx, VIRGL_SET_VIEWPORT_STATE_TRANSLATE_0(v) + i));
   }

   vrend_set_viewport_states(ctx->grctx, start_slot, num_viewports, vps);
   return 0;
}
