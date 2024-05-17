bool GLES2DecoderImpl::IsDrawValid(
    const char* function_name, GLuint max_vertex_accessed, GLsizei primcount) {
  if (!state_.current_program.get()) {
    LOCAL_RENDER_WARNING("Drawing with no current shader program.");
    return false;
  }

  return state_.vertex_attrib_manager
      ->ValidateBindings(function_name,
                         this,
                         feature_info_.get(),
                         state_.current_program.get(),
                         max_vertex_accessed,
                         primcount);
}
