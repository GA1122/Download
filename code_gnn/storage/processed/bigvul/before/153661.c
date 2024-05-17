void GLES2Implementation::GetActiveUniform(GLuint program,
                                           GLuint index,
                                           GLsizei bufsize,
                                           GLsizei* length,
                                           GLint* size,
                                           GLenum* type,
                                           char* name) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glGetActiveUniform(" << program
                     << ", " << index << ", " << bufsize << ", "
                     << static_cast<const void*>(length) << ", "
                     << static_cast<const void*>(size) << ", "
                     << static_cast<const void*>(type) << ", "
                     << static_cast<const void*>(name) << ", ");
  if (bufsize < 0) {
    SetGLError(GL_INVALID_VALUE, "glGetActiveUniform", "bufsize < 0");
    return;
  }
  TRACE_EVENT0("gpu", "GLES2::GetActiveUniform");
  bool success = share_group_->program_info_manager()->GetActiveUniform(
      this, program, index, bufsize, length, size, type, name);
  if (success) {
    if (size) {
      GPU_CLIENT_LOG("  size: " << *size);
    }
    if (type) {
      GPU_CLIENT_LOG("  type: " << GLES2Util::GetStringEnum(*type));
    }
    if (name) {
      GPU_CLIENT_LOG("  name: " << name);
    }
  }
  CheckGLError();
}