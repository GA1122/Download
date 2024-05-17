void GLES2DecoderImpl::DoLinkProgram(GLuint program_id) {
  TRACE_EVENT0("gpu", "GLES2DecoderImpl::DoLinkProgram");
  Program* program = GetProgramInfoNotShader(
      program_id, "glLinkProgram");
  if (!program) {
    return;
  }

  LogClientServiceForInfo(program, program_id, "glLinkProgram");
  ShaderTranslator* vertex_translator = NULL;
  ShaderTranslator* fragment_translator = NULL;
  if (use_shader_translator_) {
    vertex_translator = vertex_translator_.get();
    fragment_translator = fragment_translator_.get();
  }
  if (program->Link(shader_manager(),
                    vertex_translator,
                    fragment_translator,
                    workarounds().count_all_in_varyings_packing ?
                        Program::kCountAll : Program::kCountOnlyStaticallyUsed,
                    shader_cache_callback_)) {
    if (program == state_.current_program.get()) {
      if (workarounds().use_current_program_after_successful_link)
        glUseProgram(program->service_id());
      if (workarounds().clear_uniforms_before_first_program_use)
        program_manager()->ClearUniforms(program);
    }
  }
};
