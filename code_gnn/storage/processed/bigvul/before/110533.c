void GLES2DecoderImpl::ForceCompileShaderIfPending(
    ShaderManager::ShaderInfo* info) {
  if (info->compilation_status() ==
      ShaderManager::ShaderInfo::PENDING_DEFERRED_COMPILE) {

    ShaderTranslator* translator = NULL;
    if (use_shader_translator_) {
      translator = info->shader_type() == GL_VERTEX_SHADER ?
          vertex_translator_.get() : fragment_translator_.get();
    }
    program_manager()->ForceCompileShader(info->deferred_compilation_source(),
                                          info,
                                          translator,
                                          feature_info_);
  }
}
