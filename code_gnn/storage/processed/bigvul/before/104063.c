void GLES2DecoderImpl::DoUseProgram(GLuint program) {
  GLuint service_id = 0;
  ProgramManager::ProgramInfo* info = NULL;
  if (program) {
    info = GetProgramInfoNotShader(program, "glUseProgram");
    if (!info) {
      return;
    }
    if (!info->IsValid()) {
      SetGLError(GL_INVALID_OPERATION, "glUseProgram: program not linked");
      return;
    }
    service_id = info->service_id();
  }
  if (current_program_) {
    program_manager()->UnuseProgram(shader_manager(), current_program_);
  }
  current_program_ = info;
  if (current_program_) {
    program_manager()->UseProgram(current_program_);
  }
  glUseProgram(service_id);
}
