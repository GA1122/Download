bool GLES2DecoderImpl::InitializeShaderTranslator() {
  TRACE_EVENT0("gpu", "GLES2DecoderImpl::InitializeShaderTranslator");

  if (gfx::GetGLImplementation() == gfx::kGLImplementationEGLGLES2 &&
      (feature_info_->feature_flags().chromium_webglsl ||
       force_webgl_glsl_validation_) &&
      !use_shader_translator_) {
    use_shader_translator_ = true;
  }
  if (!use_shader_translator_) {
    return true;
  }
  ShBuiltInResources resources;
  ShInitBuiltInResources(&resources);
  resources.MaxVertexAttribs = group_->max_vertex_attribs();
  resources.MaxVertexUniformVectors =
      group_->max_vertex_uniform_vectors();
  resources.MaxVaryingVectors = group_->max_varying_vectors();
  resources.MaxVertexTextureImageUnits =
      group_->max_vertex_texture_image_units();
  resources.MaxCombinedTextureImageUnits = group_->max_texture_units();
  resources.MaxTextureImageUnits = group_->max_texture_image_units();
  resources.MaxFragmentUniformVectors =
      group_->max_fragment_uniform_vectors();
  resources.MaxDrawBuffers = 1;

  if (force_webgl_glsl_validation_) {
    resources.OES_standard_derivatives = derivatives_explicitly_enabled_;
  } else {
    resources.OES_standard_derivatives =
        feature_info_->feature_flags().oes_standard_derivatives ? 1 : 0;
    resources.ARB_texture_rectangle =
        feature_info_->feature_flags().arb_texture_rectangle ? 1 : 0;
  }

  ShShaderSpec shader_spec = force_webgl_glsl_validation_ ||
      feature_info_->feature_flags().chromium_webglsl ?
          SH_WEBGL_SPEC : SH_GLES2_SPEC;
  ShaderTranslatorInterface::GlslImplementationType implementation_type =
      gfx::GetGLImplementation() == gfx::kGLImplementationEGLGLES2 ?
          ShaderTranslatorInterface::kGlslES : ShaderTranslatorInterface::kGlsl;
  ShaderTranslatorInterface::GlslBuiltInFunctionBehavior function_behavior =
      needs_glsl_built_in_function_emulation_ ?
          ShaderTranslatorInterface::kGlslBuiltInFunctionEmulated :
          ShaderTranslatorInterface::kGlslBuiltInFunctionOriginal;

  ShaderTranslatorCache* cache = ShaderTranslatorCache::GetInstance();
  vertex_translator_ = cache->GetTranslator(
      SH_VERTEX_SHADER, shader_spec, &resources,
      implementation_type, function_behavior);
  if (!vertex_translator_.get()) {
    LOG(ERROR) << "Could not initialize vertex shader translator.";
    Destroy(true);
    return false;
  }

  fragment_translator_ = cache->GetTranslator(
      SH_FRAGMENT_SHADER, shader_spec, &resources,
      implementation_type, function_behavior);
  if (!fragment_translator_.get()) {
    LOG(ERROR) << "Could not initialize fragment shader translator.";
    Destroy(true);
    return false;
  }
  return true;
}
