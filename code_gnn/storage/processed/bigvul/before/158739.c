bool GLES2DecoderImpl::InitializeShaderTranslator() {
  TRACE_EVENT0("gpu", "GLES2DecoderImpl::InitializeShaderTranslator");
  if (feature_info_->disable_shader_translator()) {
    return true;
  }
  if (vertex_translator_ || fragment_translator_) {
    DCHECK(vertex_translator_ && fragment_translator_);
    return true;
  }
  ShBuiltInResources resources;
  sh::InitBuiltInResources(&resources);
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
  resources.MaxDualSourceDrawBuffers = group_->max_dual_source_draw_buffers();

  if (!feature_info_->IsWebGL1OrES2Context()) {
    resources.MaxVertexOutputVectors =
        group_->max_vertex_output_components() / 4;
    resources.MaxFragmentInputVectors =
        group_->max_fragment_input_components() / 4;
    resources.MaxProgramTexelOffset = group_->max_program_texel_offset();
    resources.MinProgramTexelOffset = group_->min_program_texel_offset();
  }

  resources.FragmentPrecisionHigh = has_fragment_precision_high_;

  ShShaderSpec shader_spec;
  switch (feature_info_->context_type()) {
    case CONTEXT_TYPE_WEBGL1:
      shader_spec = SH_WEBGL_SPEC;
      resources.OES_standard_derivatives = derivatives_explicitly_enabled_;
      resources.EXT_frag_depth = frag_depth_explicitly_enabled_;
      resources.EXT_draw_buffers = draw_buffers_explicitly_enabled_;
      if (!draw_buffers_explicitly_enabled_)
        resources.MaxDrawBuffers = 1;
      resources.EXT_shader_texture_lod = shader_texture_lod_explicitly_enabled_;
      resources.NV_draw_buffers =
          draw_buffers_explicitly_enabled_ && features().nv_draw_buffers;
      break;
    case CONTEXT_TYPE_WEBGL2:
      shader_spec = SH_WEBGL2_SPEC;
      break;
    case CONTEXT_TYPE_OPENGLES2:
      shader_spec = SH_GLES2_SPEC;
      resources.OES_standard_derivatives =
          features().oes_standard_derivatives ? 1 : 0;
      resources.ARB_texture_rectangle =
          features().arb_texture_rectangle ? 1 : 0;
      resources.OES_EGL_image_external =
          features().oes_egl_image_external ? 1 : 0;
      resources.NV_EGL_stream_consumer_external =
          features().nv_egl_stream_consumer_external ? 1 : 0;
      resources.EXT_draw_buffers =
          features().ext_draw_buffers ? 1 : 0;
      resources.EXT_frag_depth =
          features().ext_frag_depth ? 1 : 0;
      resources.EXT_shader_texture_lod =
          features().ext_shader_texture_lod ? 1 : 0;
      resources.NV_draw_buffers =
          features().nv_draw_buffers ? 1 : 0;
      resources.EXT_blend_func_extended =
          features().ext_blend_func_extended ? 1 : 0;
      break;
    case CONTEXT_TYPE_OPENGLES3:
      shader_spec = SH_GLES3_SPEC;
      resources.ARB_texture_rectangle =
          features().arb_texture_rectangle ? 1 : 0;
      resources.OES_EGL_image_external =
          features().oes_egl_image_external ? 1 : 0;
      resources.NV_EGL_stream_consumer_external =
          features().nv_egl_stream_consumer_external ? 1 : 0;
      resources.EXT_blend_func_extended =
          features().ext_blend_func_extended ? 1 : 0;
      break;
    default:
      NOTREACHED();
      shader_spec = SH_GLES2_SPEC;
      break;
  }

  if (((shader_spec == SH_WEBGL_SPEC || shader_spec == SH_WEBGL2_SPEC) &&
       features().enable_shader_name_hashing) ||
      force_shader_name_hashing_for_test)
    resources.HashFunction = &CityHash64;
  else
    resources.HashFunction = nullptr;

  ShCompileOptions driver_bug_workarounds = 0;
  if (workarounds().init_gl_position_in_vertex_shader)
    driver_bug_workarounds |= SH_INIT_GL_POSITION;
  if (workarounds().unfold_short_circuit_as_ternary_operation)
    driver_bug_workarounds |= SH_UNFOLD_SHORT_CIRCUIT;
  if (workarounds().scalarize_vec_and_mat_constructor_args)
    driver_bug_workarounds |= SH_SCALARIZE_VEC_AND_MAT_CONSTRUCTOR_ARGS;
  if (workarounds().regenerate_struct_names)
    driver_bug_workarounds |= SH_REGENERATE_STRUCT_NAMES;
  if (workarounds().remove_pow_with_constant_exponent)
    driver_bug_workarounds |= SH_REMOVE_POW_WITH_CONSTANT_EXPONENT;
  if (workarounds().emulate_abs_int_function)
    driver_bug_workarounds |= SH_EMULATE_ABS_INT_FUNCTION;
  if (workarounds().rewrite_texelfetchoffset_to_texelfetch)
    driver_bug_workarounds |= SH_REWRITE_TEXELFETCHOFFSET_TO_TEXELFETCH;
  if (workarounds().add_and_true_to_loop_condition)
    driver_bug_workarounds |= SH_ADD_AND_TRUE_TO_LOOP_CONDITION;
  if (workarounds().rewrite_do_while_loops)
    driver_bug_workarounds |= SH_REWRITE_DO_WHILE_LOOPS;
  if (workarounds().emulate_isnan_on_float)
    driver_bug_workarounds |= SH_EMULATE_ISNAN_FLOAT_FUNCTION;
  if (workarounds().use_unused_standard_shared_blocks)
    driver_bug_workarounds |= SH_USE_UNUSED_STANDARD_SHARED_BLOCKS;
  if (workarounds().dont_remove_invariant_for_fragment_input)
    driver_bug_workarounds |= SH_DONT_REMOVE_INVARIANT_FOR_FRAGMENT_INPUT;
  if (workarounds().remove_invariant_and_centroid_for_essl3)
    driver_bug_workarounds |= SH_REMOVE_INVARIANT_AND_CENTROID_FOR_ESSL3;
  if (workarounds().rewrite_float_unary_minus_operator)
    driver_bug_workarounds |= SH_REWRITE_FLOAT_UNARY_MINUS_OPERATOR;
  if (workarounds().dont_use_loops_to_initialize_variables)
    driver_bug_workarounds |= SH_DONT_USE_LOOPS_TO_INITIALIZE_VARIABLES;

  if (!workarounds().dont_initialize_uninitialized_locals)
    driver_bug_workarounds |= SH_INITIALIZE_UNINITIALIZED_LOCALS;

  resources.WEBGL_debug_shader_precision =
      group_->gpu_preferences().emulate_shader_precision;

  ShShaderOutput shader_output_language =
      ShaderTranslator::GetShaderOutputLanguageForContext(gl_version_info());

  vertex_translator_ = shader_translator_cache()->GetTranslator(
      GL_VERTEX_SHADER, shader_spec, &resources, shader_output_language,
      driver_bug_workarounds);
  if (!vertex_translator_.get()) {
    LOG(ERROR) << "Could not initialize vertex shader translator.";
    Destroy(true);
    return false;
  }

  fragment_translator_ = shader_translator_cache()->GetTranslator(
      GL_FRAGMENT_SHADER, shader_spec, &resources, shader_output_language,
      driver_bug_workarounds);
  if (!fragment_translator_.get()) {
    LOG(ERROR) << "Could not initialize fragment shader translator.";
    Destroy(true);
    return false;
  }
  return true;
}
