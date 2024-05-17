void GLES2DecoderImpl::DoLinkProgram(GLuint program_id) {
  TRACE_EVENT0("gpu", "GLES2DecoderImpl::DoLinkProgram");
  SCOPED_UMA_HISTOGRAM_TIMER("GPU.DoLinkProgramTime");
  Program* program = GetProgramInfoNotShader(
      program_id, "glLinkProgram");
  if (!program) {
    return;
  }

  LogClientServiceForInfo(program, program_id, "glLinkProgram");
  if (program->Link(shader_manager(),
                    workarounds().count_all_in_varyings_packing
                        ? Program::kCountAll
                        : Program::kCountOnlyStaticallyUsed,
                    client_)) {
    if (program == state_.current_program.get()) {
      if (workarounds().clear_uniforms_before_first_program_use)
        program_manager()->ClearUniforms(program);
    }
  }

  ExitCommandProcessingEarly();
}
