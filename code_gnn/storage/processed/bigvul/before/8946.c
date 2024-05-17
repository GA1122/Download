void vrend_set_viewport_states(struct vrend_context *ctx,
                               uint32_t start_slot,
                               uint32_t num_viewports,
                               const struct pipe_viewport_state *state)
{
    
   GLint x, y;
   GLsizei width, height;
   GLclampd near_val, far_val;
   bool viewport_is_negative = (state[0].scale[1] < 0) ? true : false;
   int i, idx;

   if (num_viewports > PIPE_MAX_VIEWPORTS ||
       start_slot > (PIPE_MAX_VIEWPORTS - num_viewports)) {
      report_context_error(ctx, VIRGL_ERROR_CTX_ILLEGAL_CMD_BUFFER, num_viewports);
      return;
   }

   for (i = 0; i < num_viewports; i++) {
      GLfloat abs_s1 = fabsf(state[i].scale[1]);

      idx = start_slot + i;
      width = state[i].scale[0] * 2.0f;
      height = abs_s1 * 2.0f;
      x = state[i].translate[0] - state[i].scale[0];
      y = state[i].translate[1] - state[i].scale[1];

      near_val = state[i].translate[2] - state[i].scale[2];
      far_val = near_val + (state[i].scale[2] * 2.0);

      if (ctx->sub->vps[idx].cur_x != x ||
          ctx->sub->vps[idx].cur_y != y ||
          ctx->sub->vps[idx].width != width ||
          ctx->sub->vps[idx].height != height) {
         ctx->sub->viewport_state_dirty |= (1 << idx);
         ctx->sub->vps[idx].cur_x = x;
         ctx->sub->vps[idx].cur_y = y;
         ctx->sub->vps[idx].width = width;
         ctx->sub->vps[idx].height = height;
      }

      if (idx == 0) {
         if (ctx->sub->viewport_is_negative != viewport_is_negative)
            ctx->sub->viewport_is_negative = viewport_is_negative;

         ctx->sub->depth_scale = fabsf(far_val - near_val);
         ctx->sub->depth_transform = near_val;
      }

      if (ctx->sub->vps[idx].near_val != near_val ||
          ctx->sub->vps[idx].far_val != far_val) {
         ctx->sub->vps[idx].near_val = near_val;
         ctx->sub->vps[idx].far_val = far_val;
         if (idx)
            glDepthRangeIndexed(idx, ctx->sub->vps[idx].near_val, ctx->sub->vps[idx].far_val);
         else
            glDepthRange(ctx->sub->vps[idx].near_val, ctx->sub->vps[idx].far_val);
      }
   }
}
