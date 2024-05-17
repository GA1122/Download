void GLES2DecoderImpl::DoLinkProgram(GLuint program) {
  TRACE_EVENT0("gpu", "GLES2DecoderImpl::DoLinkProgram");
  ProgramManager::ProgramInfo* info = GetProgramInfoNotShader(
      program, "glLinkProgram");
  if (!info) {
    return;
  }

  ShaderTranslator* vertex_translator = NULL;
  ShaderTranslator* fragment_translator = NULL;
  if (use_shader_translator_) {
    vertex_translator = vertex_translator_;
    fragment_translator = fragment_translator_;
  }
  if (info->Link(shader_manager(),
                 vertex_translator,
                 fragment_translator,
                 feature_info_)) {
    if (info == current_program_.get()) {
      program_manager()->ClearUniforms(info);
    }
  }
};
