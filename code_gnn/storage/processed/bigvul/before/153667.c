void GLES2Implementation::GetActiveUniformsiv(GLuint program,
                                              GLsizei count,
                                              const GLuint* indices,
                                              GLenum pname,
                                              GLint* params) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glGetActiveUniformsiv(" << program
                     << ", " << count << ", "
                     << static_cast<const void*>(indices) << ", "
                     << GLES2Util::GetStringUniformParameter(pname) << ", "
                     << static_cast<const void*>(params) << ")");
  TRACE_EVENT0("gpu", "GLES2::GetActiveUniformsiv");
  if (count < 0) {
    SetGLError(GL_INVALID_VALUE, "glGetActiveUniformsiv", "count < 0");
    return;
  }
  bool success = share_group_->program_info_manager()->GetActiveUniformsiv(
      this, program, count, indices, pname, params);
  if (success) {
    if (params) {
      GPU_CLIENT_LOG_CODE_BLOCK({
        for (GLsizei ii = 0; ii < count; ++ii) {
          GPU_CLIENT_LOG("  " << ii << ": " << params[ii]);
        }
      });
    }
  }
  CheckGLError();
}
