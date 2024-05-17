bool GLES2DecoderImpl::IsDrawValid(
    const char* function_name, GLuint max_vertex_accessed, bool instanced,
    GLsizei primcount) {
  DCHECK(instanced || primcount == 1);

  if (!state_.current_program.get()) {
    LOCAL_RENDER_WARNING("Drawing with no current shader program.");
    return false;
  }

  if (CheckDrawingFeedbackLoops()) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION, function_name,
        "Source and destination textures of the draw are the same.");
    return false;
  }

  return state_.vertex_attrib_manager
      ->ValidateBindings(function_name,
                         this,
                         feature_info_.get(),
                         state_.current_program.get(),
                         max_vertex_accessed,
                         instanced,
                         primcount);
}
