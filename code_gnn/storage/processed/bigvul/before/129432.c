error::Error GLES2DecoderImpl::HandleRequestExtensionCHROMIUM(
    uint32 immediate_data_size, const cmds::RequestExtensionCHROMIUM& c) {
  Bucket* bucket = GetBucket(c.bucket_id);
  if (!bucket || bucket->size() == 0) {
    return error::kInvalidArguments;
  }
  std::string feature_str;
  if (!bucket->GetAsString(&feature_str)) {
    return error::kInvalidArguments;
  }

  bool desire_webgl_glsl_validation =
      feature_str.find("GL_CHROMIUM_webglsl") != std::string::npos;
  bool desire_standard_derivatives = false;
  bool desire_frag_depth = false;
  bool desire_draw_buffers = false;
  bool desire_shader_texture_lod = false;
  if (force_webgl_glsl_validation_) {
    desire_standard_derivatives =
        feature_str.find("GL_OES_standard_derivatives") != std::string::npos;
    desire_frag_depth =
        feature_str.find("GL_EXT_frag_depth") != std::string::npos;
    desire_draw_buffers =
        feature_str.find("GL_EXT_draw_buffers") != std::string::npos;
    desire_shader_texture_lod =
        feature_str.find("GL_EXT_shader_texture_lod") != std::string::npos;
  }

  if (desire_webgl_glsl_validation != force_webgl_glsl_validation_ ||
      desire_standard_derivatives != derivatives_explicitly_enabled_ ||
      desire_frag_depth != frag_depth_explicitly_enabled_ ||
      desire_draw_buffers != draw_buffers_explicitly_enabled_) {
    force_webgl_glsl_validation_ |= desire_webgl_glsl_validation;
    derivatives_explicitly_enabled_ |= desire_standard_derivatives;
    frag_depth_explicitly_enabled_ |= desire_frag_depth;
    draw_buffers_explicitly_enabled_ |= desire_draw_buffers;
    shader_texture_lod_explicitly_enabled_ |= desire_shader_texture_lod;
    InitializeShaderTranslator();
  }

  UpdateCapabilities();

  return error::kNoError;
}
