void vrend_render_condition(struct vrend_context *ctx,
                            uint32_t handle,
                            bool condition,
                            uint mode)
{
   struct vrend_query *q;
   GLenum glmode = 0;

   if (handle == 0) {
      glEndConditionalRenderNV();
      ctx->sub->cond_render_q_id = 0;
      ctx->sub->cond_render_gl_mode = 0;
      return;
   }

   q = vrend_object_lookup(ctx->sub->object_hash, handle, VIRGL_OBJECT_QUERY);
   if (!q)
      return;

   switch (mode) {
   case PIPE_RENDER_COND_WAIT:
      glmode = GL_QUERY_WAIT;
      break;
   case PIPE_RENDER_COND_NO_WAIT:
      glmode = GL_QUERY_NO_WAIT;
      break;
   case PIPE_RENDER_COND_BY_REGION_WAIT:
      glmode = GL_QUERY_BY_REGION_WAIT;
      break;
   case PIPE_RENDER_COND_BY_REGION_NO_WAIT:
      glmode = GL_QUERY_BY_REGION_NO_WAIT;
      break;
   default:
      fprintf(stderr, "unhandled condition %x\n", mode);
   }

   ctx->sub->cond_render_q_id = q->id;
   ctx->sub->cond_render_gl_mode = glmode;
   glBeginConditionalRender(q->id, glmode);

}
