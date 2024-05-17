void GLES2DecoderImpl::DoUseProgram(GLuint program_id) {
  const char* function_name = "glUseProgram";
  GLuint service_id = 0;
  Program* program = nullptr;
  if (program_id) {
    program = GetProgramInfoNotShader(program_id, function_name);
    if (!program) {
      return;
    }
    if (!program->IsValid()) {
      LOCAL_SET_GL_ERROR(
          GL_INVALID_OPERATION, function_name, "program not linked");
      return;
    }
    service_id = program->service_id();
  }
  if (state_.bound_transform_feedback.get() &&
      state_.bound_transform_feedback->active() &&
      !state_.bound_transform_feedback->paused()) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, function_name,
        "transformfeedback is active and not paused");
    return;
  }
  if (program == state_.current_program.get())
    return;
  if (state_.current_program.get()) {
    program_manager()->UnuseProgram(shader_manager(),
                                    state_.current_program.get());
  }
  state_.current_program = program;
  LogClientServiceMapping(function_name, program_id, service_id);
  api()->glUseProgramFn(service_id);
  if (state_.current_program.get()) {
    program_manager()->UseProgram(state_.current_program.get());
    if (workarounds().clear_uniforms_before_first_program_use)
      program_manager()->ClearUniforms(program);
  }
}
