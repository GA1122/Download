bool GLES2DecoderImpl::InitializeShaderTranslator() {
  TRACE_EVENT0("gpu", "GLES2DecoderImpl::InitializeShaderTranslator");

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
  resources.MaxDrawBuffers = group_->max_draw_buffers();
  resources.MaxExpressionComplexity = 256;
  resources.MaxCallStackDepth = 256;

  GLint range[2] = { 0, 0 };
  GLint precision = 0;
  GetShaderPrecisionFormatImpl(GL_FRAGMENT_SHADER, GL_HIGH_FLOAT,
                               range, &precision);
  resources.FragmentPrecisionHigh =
      PrecisionMeetsSpecForHighpFloat(range[0], range[1], precision);

  if (force_webgl_glsl_validation_) {
    resources.OES_standard_derivatives = derivatives_explicitly_enabled_;
    resources.EXT_frag_depth = frag_depth_explicitly_enabled_;
    resources.EXT_draw_buffers = draw_buffers_explicitly_enabled_;
    if (!draw_buffers_explicitly_enabled_)
      resources.MaxDrawBuffers = 1;
    resources.EXT_shader_texture_lod = shader_texture_lod_explicitly_enabled_;
    resources.NV_draw_buffers =
        draw_buffers_explicitly_enabled_ && features().nv_draw_buffers;
  } else {
    resources.OES_standard_derivatives =
        features().oes_standard_derivatives ? 1 : 0;
    resources.ARB_texture_rectangle =
        features().arb_texture_rectangle ? 1 : 0;
    resources.OES_EGL_image_external =
        features().oes_egl_image_external ? 1 : 0;
    resources.EXT_draw_buffers =
        features().ext_draw_buffers ? 1 : 0;
    resources.EXT_frag_depth =
        features().ext_frag_depth ? 1 : 0;
    resources.EXT_shader_texture_lod =
        features().ext_shader_texture_lod ? 1 : 0;
    resources.NV_draw_buffers =
        features().nv_draw_buffers ? 1 : 0;
  }

  ShShaderSpec shader_spec;
  if (force_webgl_glsl_validation_) {
    shader_spec = unsafe_es3_apis_enabled() ? SH_WEBGL2_SPEC : SH_WEBGL_SPEC;
  } else {
    shader_spec = unsafe_es3_apis_enabled() ? SH_GLES3_SPEC : SH_GLES2_SPEC;
  }

  if ((shader_spec == SH_WEBGL_SPEC || shader_spec == SH_WEBGL2_SPEC) &&
      features().enable_shader_name_hashing)
    resources.HashFunction = &CityHash64;
  else
    resources.HashFunction = NULL;
  ShaderTranslatorInterface::GlslImplementationType implementation_type =
      gfx::GetGLImplementation() == gfx::kGLImplementationEGLGLES2 ?
          ShaderTranslatorInterface::kGlslES : ShaderTranslatorInterface::kGlsl;
  int driver_bug_workarounds = 0;
  if (workarounds().needs_glsl_built_in_function_emulation)
    driver_bug_workarounds |= SH_EMULATE_BUILT_IN_FUNCTIONS;
  if (workarounds().init_gl_position_in_vertex_shader)
    driver_bug_workarounds |= SH_INIT_GL_POSITION;
  if (workarounds().unfold_short_circuit_as_ternary_operation)
    driver_bug_workarounds |= SH_UNFOLD_SHORT_CIRCUIT;
  if (workarounds().init_varyings_without_static_use)
    driver_bug_workarounds |= SH_INIT_VARYINGS_WITHOUT_STATIC_USE;
  if (workarounds().unroll_for_loop_with_sampler_array_index)
    driver_bug_workarounds |= SH_UNROLL_FOR_LOOP_WITH_SAMPLER_ARRAY_INDEX;
  if (workarounds().scalarize_vec_and_mat_constructor_args)
    driver_bug_workarounds |= SH_SCALARIZE_VEC_AND_MAT_CONSTRUCTOR_ARGS;
  if (workarounds().regenerate_struct_names)
    driver_bug_workarounds |= SH_REGENERATE_STRUCT_NAMES;

  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kEmulateShaderPrecision))
    resources.WEBGL_debug_shader_precision = true;

  vertex_translator_ = shader_translator_cache()->GetTranslator(
      GL_VERTEX_SHADER,
      shader_spec,
      &resources,
      implementation_type,
      static_cast<ShCompileOptions>(driver_bug_workarounds));
  if (!vertex_translator_.get()) {
    LOG(ERROR) << "Could not initialize vertex shader translator.";
    Destroy(true);
    return false;
  }

  fragment_translator_ = shader_translator_cache()->GetTranslator(
      GL_FRAGMENT_SHADER,
      shader_spec,
      &resources,
      implementation_type,
      static_cast<ShCompileOptions>(driver_bug_workarounds));
  if (!fragment_translator_.get()) {
    LOG(ERROR) << "Could not initialize fragment shader translator.";
    Destroy(true);
    return false;
  }
  return true;
}
