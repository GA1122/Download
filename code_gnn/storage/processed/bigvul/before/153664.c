void GLES2Implementation::GetActiveUniformBlockiv(GLuint program,
                                                  GLuint index,
                                                  GLenum pname,
                                                  GLint* params) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glGetActiveUniformBlockiv("
                     << program << ", " << index << ", "
                     << GLES2Util::GetStringUniformBlockParameter(pname) << ", "
                     << static_cast<const void*>(params) << ")");
  TRACE_EVENT0("gpu", "GLES2::GetActiveUniformBlockiv");
  bool success = share_group_->program_info_manager()->GetActiveUniformBlockiv(
      this, program, index, pname, params);
  if (success) {
    if (params) {
      GPU_CLIENT_LOG("  params: " << params[0]);
    }
  }
  CheckGLError();
}
