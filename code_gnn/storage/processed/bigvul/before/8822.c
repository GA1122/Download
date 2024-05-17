void vrend_bind_sampler_states(struct vrend_context *ctx,
                               uint32_t shader_type,
                               uint32_t start_slot,
                               uint32_t num_states,
                               uint32_t *handles)
{
   int i;
   struct vrend_sampler_state *state;

   if (shader_type >= PIPE_SHADER_TYPES) {
      report_context_error(ctx, VIRGL_ERROR_CTX_ILLEGAL_CMD_BUFFER, shader_type);
      return;
   }

   if (num_states > PIPE_MAX_SAMPLERS ||
       start_slot > (PIPE_MAX_SAMPLERS - num_states)) {
      report_context_error(ctx, VIRGL_ERROR_CTX_ILLEGAL_CMD_BUFFER, num_states);
      return;
   }

   ctx->sub->num_sampler_states[shader_type] = num_states;

   for (i = 0; i < num_states; i++) {
      if (handles[i] == 0)
         state = NULL;
      else
         state = vrend_object_lookup(ctx->sub->object_hash, handles[i], VIRGL_OBJECT_SAMPLER_STATE);

      ctx->sub->sampler_state[shader_type][i + start_slot] = state;
   }
   ctx->sub->sampler_state_dirty = true;
}
