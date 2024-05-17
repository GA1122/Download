GLuint GLES2Implementation::GetUniformBlockIndex(GLuint program,
                                                 const char* name) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glGetUniformBlockIndex(" << program
                     << ", " << name << ")");
  TRACE_EVENT0("gpu", "GLES2::GetUniformBlockIndex");
  GLuint index = share_group_->program_info_manager()->GetUniformBlockIndex(
      this, program, name);
  GPU_CLIENT_LOG("returned " << index);
  CheckGLError();
  return index;
}
