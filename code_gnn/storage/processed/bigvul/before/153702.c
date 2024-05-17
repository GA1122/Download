GLuint GLES2Implementation::GetProgramResourceIndex(
    GLuint program,
    GLenum program_interface,
    const char* name) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glGetProgramResourceIndex("
                     << program << ", " << program_interface << ", " << name
                     << ")");
  TRACE_EVENT0("gpu", "GLES2::GetProgramResourceIndex");
  GLuint index = share_group_->program_info_manager()->GetProgramResourceIndex(
      this, program, program_interface, name);
  GPU_CLIENT_LOG("returned " << index);
  CheckGLError();
  return index;
}
