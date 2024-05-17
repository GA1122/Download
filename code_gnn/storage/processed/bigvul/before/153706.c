void GLES2Implementation::GetProgramResourceName(GLuint program,
                                                 GLenum program_interface,
                                                 GLuint index,
                                                 GLsizei bufsize,
                                                 GLsizei* length,
                                                 char* name) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glGetProgramResourceName("
                     << program << ", " << program_interface << ", " << index
                     << ", " << bufsize << ", " << static_cast<void*>(length)
                     << ", " << static_cast<void*>(name) << ")");
  if (bufsize < 0) {
    SetGLError(GL_INVALID_VALUE, "glGetProgramResourceName", "bufsize < 0");
    return;
  }
  TRACE_EVENT0("gpu", "GLES2::GetProgramResourceName");
  bool success = share_group_->program_info_manager()->GetProgramResourceName(
      this, program, program_interface, index, bufsize, length, name);
  if (success && name) {
    GPU_CLIENT_LOG("  name: " << name);
  }
  CheckGLError();
}
