void GLES2Implementation::GetUniformIndices(GLuint program,
                                            GLsizei count,
                                            const char* const* names,
                                            GLuint* indices) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glGetUniformIndices(" << program
                     << ", " << count << ", " << names << ", " << indices
                     << ")");
  TRACE_EVENT0("gpu", "GLES2::GetUniformIndices");
  if (count < 0) {
    SetGLError(GL_INVALID_VALUE, "glGetUniformIndices", "count < 0");
    return;
  }
  if (count == 0) {
    return;
  }
  bool success = share_group_->program_info_manager()->GetUniformIndices(
      this, program, count, names, indices);
  if (success) {
    GPU_CLIENT_LOG_CODE_BLOCK({
      for (GLsizei ii = 0; ii < count; ++ii) {
        GPU_CLIENT_LOG("  " << ii << ": " << indices[ii]);
      }
    });
  }
  CheckGLError();
}
