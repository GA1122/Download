void GLES2DecoderImpl::DoUseProgram(GLuint program_id) {
  GLuint service_id = 0;
  Program* program = NULL;
  if (program_id) {
    program = GetProgramInfoNotShader(program_id, "glUseProgram");
    if (!program) {
      return;
    }
    if (!program->IsValid()) {
      LOCAL_SET_GL_ERROR(
          GL_INVALID_OPERATION, "glUseProgram", "program not linked");
      return;
    }
    service_id = program->service_id();
  }
  if (state_.current_program.get()) {
    program_manager()->UnuseProgram(shader_manager(),
                                    state_.current_program.get());
  }
  state_.current_program = program;
  LogClientServiceMapping("glUseProgram", program_id, service_id);
  glUseProgram(service_id);
  if (state_.current_program.get()) {
    program_manager()->UseProgram(state_.current_program.get());
    if (workarounds().clear_uniforms_before_first_program_use)
      program_manager()->ClearUniforms(program);
  }
}
