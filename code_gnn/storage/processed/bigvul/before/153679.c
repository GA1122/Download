GLint GLES2Implementation::GetFragDataIndexEXT(GLuint program,
                                               const char* name) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glGetFragDataIndexEXT(" << program
                     << ", " << name << ")");
  TRACE_EVENT0("gpu", "GLES2::GetFragDataIndexEXT");
  GLint loc = share_group_->program_info_manager()->GetFragDataIndex(
      this, program, name);
  GPU_CLIENT_LOG("returned " << loc);
  CheckGLError();
  return loc;
}
