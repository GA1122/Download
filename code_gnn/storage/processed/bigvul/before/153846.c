bool GLES2Implementation::UpdateIndexedBufferState(GLenum target,
                                                   GLuint index,
                                                   GLuint buffer_id,
                                                   const char* function_name) {
  switch (target) {
    case GL_ATOMIC_COUNTER_BUFFER:
      if (index >= static_cast<GLuint>(
                       capabilities_.max_atomic_counter_buffer_bindings)) {
        SetGLError(GL_INVALID_VALUE, function_name, "index out of range");
        return false;
      }
      bound_atomic_counter_buffer_ = buffer_id;
      break;
    case GL_TRANSFORM_FEEDBACK_BUFFER:
      if (index >= static_cast<GLuint>(
                       capabilities_.max_transform_feedback_separate_attribs)) {
        SetGLError(GL_INVALID_VALUE, function_name, "index out of range");
        return false;
      }
      bound_transform_feedback_buffer_ = buffer_id;
      break;
    case GL_SHADER_STORAGE_BUFFER:
      if (index >= static_cast<GLuint>(
                       capabilities_.max_shader_storage_buffer_bindings)) {
        SetGLError(GL_INVALID_VALUE, function_name, "index out of range");
        return false;
      }
      bound_shader_storage_buffer_ = buffer_id;
      break;
    case GL_UNIFORM_BUFFER:
      if (index >=
          static_cast<GLuint>(capabilities_.max_uniform_buffer_bindings)) {
        SetGLError(GL_INVALID_VALUE, function_name, "index out of range");
        return false;
      }
      bound_uniform_buffer_ = buffer_id;
      break;
    default:
      SetGLError(GL_INVALID_ENUM, function_name, "invalid target");
      return false;
  }
  return true;
}
