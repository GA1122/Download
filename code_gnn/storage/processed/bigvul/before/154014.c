GLuint GLES2DecoderImpl::DoGetMaxValueInBufferCHROMIUM(
    GLuint buffer_id, GLsizei count, GLenum type, GLuint offset) {
  GLuint max_vertex_accessed = 0;
  Buffer* buffer = GetBuffer(buffer_id);
  if (!buffer) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_VALUE, "GetMaxValueInBufferCHROMIUM", "unknown buffer");
  } else {
    if (!buffer->GetMaxValueForRange(
            offset, count, type,
            state_.enable_flags.primitive_restart_fixed_index,
            &max_vertex_accessed)) {
      LOCAL_SET_GL_ERROR(
          GL_INVALID_OPERATION,
          "GetMaxValueInBufferCHROMIUM", "range out of bounds for buffer");
    }
  }
  return max_vertex_accessed;
}
