GLint GLES2Implementation::GetFragDataLocation(GLuint program,
                                               const char* name) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glGetFragDataLocation(" << program
                     << ", " << name << ")");
  TRACE_EVENT0("gpu", "GLES2::GetFragDataLocation");
  GLint loc = share_group_->program_info_manager()->GetFragDataLocation(
      this, program, name);
  GPU_CLIENT_LOG("returned " << loc);
  CheckGLError();
  return loc;
}
