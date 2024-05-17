void GLES2Implementation::ShaderBinary(GLsizei n,
                                       const GLuint* shaders,
                                       GLenum binaryformat,
                                       const void* binary,
                                       GLsizei length) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glShaderBinary(" << n << ", "
                     << static_cast<const void*>(shaders) << ", "
                     << GLES2Util::GetStringEnum(binaryformat) << ", "
                     << static_cast<const void*>(binary) << ", " << length
                     << ")");
  if (n < 0) {
    SetGLError(GL_INVALID_VALUE, "glShaderBinary", "n < 0.");
    return;
  }
  if (length < 0) {
    SetGLError(GL_INVALID_VALUE, "glShaderBinary", "length < 0.");
    return;
  }
  unsigned int shader_id_size = n * sizeof(*shaders);
  ScopedTransferBufferArray<GLint> buffer(shader_id_size + length, helper_,
                                          transfer_buffer_);
  if (!buffer.valid() || buffer.num_elements() != shader_id_size + length) {
    SetGLError(GL_OUT_OF_MEMORY, "glShaderBinary", "out of memory.");
    return;
  }
  void* shader_ids = buffer.elements();
  void* shader_data = buffer.elements() + shader_id_size;
  memcpy(shader_ids, shaders, shader_id_size);
  memcpy(shader_data, binary, length);
  helper_->ShaderBinary(n, buffer.shm_id(), buffer.offset(), binaryformat,
                        buffer.shm_id(), buffer.offset() + shader_id_size,
                        length);
  CheckGLError();
}
