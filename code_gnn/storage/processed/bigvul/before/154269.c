error::Error GLES2DecoderImpl::HandleRequestExtensionCHROMIUM(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  const volatile gles2::cmds::RequestExtensionCHROMIUM& c =
      *static_cast<const volatile gles2::cmds::RequestExtensionCHROMIUM*>(
          cmd_data);
  Bucket* bucket = GetBucket(c.bucket_id);
  if (!bucket || bucket->size() == 0) {
    return error::kInvalidArguments;
  }
  std::string feature_str;
  if (!bucket->GetAsString(&feature_str)) {
    return error::kInvalidArguments;
  }
  feature_str = feature_str + " ";

  bool desire_standard_derivatives = false;
  bool desire_frag_depth = false;
  bool desire_draw_buffers = false;
  bool desire_shader_texture_lod = false;
  bool desire_multi_draw = false;
  bool desire_multi_draw_instanced = false;
  if (feature_info_->context_type() == CONTEXT_TYPE_WEBGL1) {
    desire_standard_derivatives =
        feature_str.find("GL_OES_standard_derivatives ") != std::string::npos;
    desire_frag_depth =
        feature_str.find("GL_EXT_frag_depth ") != std::string::npos;
    desire_draw_buffers =
        feature_str.find("GL_EXT_draw_buffers ") != std::string::npos;
    desire_shader_texture_lod =
        feature_str.find("GL_EXT_shader_texture_lod ") != std::string::npos;
  }
  if (feature_info_->IsWebGLContext()) {
    desire_multi_draw =
        feature_str.find("GL_WEBGL_multi_draw ") != std::string::npos;
    desire_multi_draw_instanced =
        feature_str.find("GL_WEBGL_multi_draw_instanced ") != std::string::npos;
  }
  if (desire_standard_derivatives != derivatives_explicitly_enabled_ ||
      desire_frag_depth != frag_depth_explicitly_enabled_ ||
      desire_draw_buffers != draw_buffers_explicitly_enabled_ ||
      desire_shader_texture_lod != shader_texture_lod_explicitly_enabled_ ||
      desire_multi_draw != multi_draw_explicitly_enabled_ ||
      desire_multi_draw_instanced != multi_draw_instanced_explicitly_enabled_) {
    derivatives_explicitly_enabled_ |= desire_standard_derivatives;
    frag_depth_explicitly_enabled_ |= desire_frag_depth;
    draw_buffers_explicitly_enabled_ |= desire_draw_buffers;
    shader_texture_lod_explicitly_enabled_ |= desire_shader_texture_lod;
    multi_draw_explicitly_enabled_ |= desire_multi_draw;
    multi_draw_instanced_explicitly_enabled_ |= desire_multi_draw_instanced;
    DestroyShaderTranslator();
  }

  if (feature_str.find("GL_CHROMIUM_color_buffer_float_rgba ") !=
      std::string::npos) {
    feature_info_->EnableCHROMIUMColorBufferFloatRGBA();
  }
  if (feature_str.find("GL_CHROMIUM_color_buffer_float_rgb ") !=
      std::string::npos) {
    feature_info_->EnableCHROMIUMColorBufferFloatRGB();
  }
  if (feature_str.find("GL_EXT_color_buffer_float ") != std::string::npos) {
    feature_info_->EnableEXTColorBufferFloat();
  }
  if (feature_str.find("GL_EXT_color_buffer_half_float ") !=
      std::string::npos) {
    feature_info_->EnableEXTColorBufferHalfFloat();
  }
  if (feature_str.find("GL_OES_texture_float_linear ") != std::string::npos) {
    feature_info_->EnableOESTextureFloatLinear();
  }
  if (feature_str.find("GL_OES_texture_half_float_linear ") !=
      std::string::npos) {
    feature_info_->EnableOESTextureHalfFloatLinear();
  }
  if (feature_str.find("GL_EXT_float_blend ") != std::string::npos) {
    feature_info_->EnableEXTFloatBlend();
  }

  UpdateCapabilities();

  return error::kNoError;
}
