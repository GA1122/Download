ALWAYS_INLINE bool GLES2DecoderImpl::CheckTransformFeedback(
    const char* function_name,
    bool instanced,
    GLenum mode,
    const GLsizei* counts,
    const GLsizei* primcounts,
    GLsizei drawcount,
    GLsizei* vertices_drawn) {
  DCHECK(state_.bound_transform_feedback.get());
  if (state_.bound_transform_feedback->active() &&
      !state_.bound_transform_feedback->paused()) {
    if (mode != state_.bound_transform_feedback->primitive_mode()) {
      LOCAL_SET_GL_ERROR(
          GL_INVALID_OPERATION, function_name,
          "mode differs from active transformfeedback's primitiveMode");
      return false;
    }
    for (GLsizei draw_id = 0; draw_id < drawcount; ++draw_id) {
      GLsizei count = counts[draw_id];
      GLsizei primcount = instanced ? primcounts[draw_id] : 1;

      bool valid = state_.bound_transform_feedback->GetVerticesNeededForDraw(
          mode, count, primcount, *vertices_drawn, vertices_drawn);
      if (!valid) {
        LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, function_name,
                           "integer overflow calculating number of vertices "
                           "for transform feedback");
        return false;
      }
    }

    if (!buffer_manager()->RequestBuffersAccess(
            error_state_.get(), state_.bound_transform_feedback.get(),
            state_.current_program->GetTransformFeedbackVaryingSizes(),
            *vertices_drawn, function_name, "transformfeedback buffers")) {
      return false;
    }
  }
  return true;
}
