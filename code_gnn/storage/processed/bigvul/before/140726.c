void GLES2DecoderImpl::DoCompileShader(GLuint client_id) {
  TRACE_EVENT0("gpu", "GLES2DecoderImpl::DoCompileShader");
  Shader* shader = GetShaderInfoNotProgram(client_id, "glCompileShader");
  if (!shader) {
    return;
  }

  scoped_refptr<ShaderTranslatorInterface> translator;
  if (use_shader_translator_) {
      translator = shader->shader_type() == GL_VERTEX_SHADER ?
                   vertex_translator_ : fragment_translator_;
  }

  const Shader::TranslatedShaderSourceType source_type =
      feature_info_->feature_flags().angle_translated_shader_source ?
      Shader::kANGLE : Shader::kGL;
  shader->RequestCompile(translator, source_type);
}
