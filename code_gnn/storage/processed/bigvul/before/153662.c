void GLES2Implementation::GetActiveUniformBlockName(GLuint program,
                                                    GLuint index,
                                                    GLsizei bufsize,
                                                    GLsizei* length,
                                                    char* name) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glGetActiveUniformBlockName("
                     << program << ", " << index << ", " << bufsize << ", "
                     << static_cast<const void*>(length) << ", "
                     << static_cast<const void*>(name) << ")");
  if (bufsize < 0) {
    SetGLError(GL_INVALID_VALUE, "glGetActiveUniformBlockName", "bufsize < 0");
    return;
  }
  TRACE_EVENT0("gpu", "GLES2::GetActiveUniformBlockName");
  bool success =
      share_group_->program_info_manager()->GetActiveUniformBlockName(
          this, program, index, bufsize, length, name);
  if (success) {
    if (name) {
      GPU_CLIENT_LOG("  name: " << name);
    }
  }
  CheckGLError();
}
