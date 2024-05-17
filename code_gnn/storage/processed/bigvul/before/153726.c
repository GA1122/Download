void GLES2Implementation::GetTransformFeedbackVarying(GLuint program,
                                                      GLuint index,
                                                      GLsizei bufsize,
                                                      GLsizei* length,
                                                      GLint* size,
                                                      GLenum* type,
                                                      char* name) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glGetTransformFeedbackVarying("
                     << program << ", " << index << ", " << bufsize << ", "
                     << static_cast<const void*>(length) << ", "
                     << static_cast<const void*>(size) << ", "
                     << static_cast<const void*>(type) << ", "
                     << static_cast<const void*>(name) << ", ");
  if (bufsize < 0) {
    SetGLError(GL_INVALID_VALUE, "glGetTransformFeedbackVarying",
               "bufsize < 0");
    return;
  }
  TRACE_EVENT0("gpu", "GLES2::GetTransformFeedbackVarying");
  bool success =
      share_group_->program_info_manager()->GetTransformFeedbackVarying(
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
