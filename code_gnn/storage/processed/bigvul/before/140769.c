Capabilities GLES2DecoderImpl::GetCapabilities() {
  DCHECK(initialized());

  Capabilities caps;
  caps.VisitPrecisions([](GLenum shader, GLenum type,
                          Capabilities::ShaderPrecision* shader_precision) {
    GLint range[2] = {0, 0};
    GLint precision = 0;
    GetShaderPrecisionFormatImpl(shader, type, range, &precision);
    shader_precision->min_range = range[0];
    shader_precision->max_range = range[1];
    shader_precision->precision = precision;
  });
  DoGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
                &caps.max_combined_texture_image_units);
  DoGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE, &caps.max_cube_map_texture_size);
  DoGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS,
                &caps.max_fragment_uniform_vectors);
  DoGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &caps.max_renderbuffer_size);
  DoGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &caps.max_texture_image_units);
  DoGetIntegerv(GL_MAX_TEXTURE_SIZE, &caps.max_texture_size);
  DoGetIntegerv(GL_MAX_VARYING_VECTORS, &caps.max_varying_vectors);
  DoGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &caps.max_vertex_attribs);
  DoGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
                &caps.max_vertex_texture_image_units);
  DoGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS,
                &caps.max_vertex_uniform_vectors);
  DoGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS,
                &caps.num_compressed_texture_formats);
  DoGetIntegerv(GL_NUM_SHADER_BINARY_FORMATS, &caps.num_shader_binary_formats);
  DoGetIntegerv(GL_BIND_GENERATES_RESOURCE_CHROMIUM,
                &caps.bind_generates_resource_chromium);
  if (unsafe_es3_apis_enabled()) {
    DoGetIntegerv(GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS,
                  &caps.max_transform_feedback_separate_attribs);
    DoGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS,
                  &caps.max_uniform_buffer_bindings);
    DoGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT,
                  &caps.uniform_buffer_offset_alignment);
  }

  caps.egl_image_external =
      feature_info_->feature_flags().oes_egl_image_external;
  caps.texture_format_atc =
      feature_info_->feature_flags().ext_texture_format_atc;
  caps.texture_format_bgra8888 =
      feature_info_->feature_flags().ext_texture_format_bgra8888;
  caps.texture_format_dxt1 =
      feature_info_->feature_flags().ext_texture_format_dxt1;
  caps.texture_format_dxt5 =
      feature_info_->feature_flags().ext_texture_format_dxt5;
  caps.texture_format_etc1 =
      feature_info_->feature_flags().oes_compressed_etc1_rgb8_texture;
  caps.texture_format_etc1_npot =
      caps.texture_format_etc1 && !workarounds().etc1_power_of_two_only;
  caps.texture_rectangle = feature_info_->feature_flags().arb_texture_rectangle;
  caps.texture_usage = feature_info_->feature_flags().angle_texture_usage;
  caps.texture_storage = feature_info_->feature_flags().ext_texture_storage;
  caps.discard_framebuffer =
      feature_info_->feature_flags().ext_discard_framebuffer;
  caps.sync_query = feature_info_->feature_flags().chromium_sync_query;

#if defined(OS_MACOSX)
  caps.iosurface = true;
#endif

  caps.post_sub_buffer = supports_post_sub_buffer_;
  caps.image = true;

  caps.blend_equation_advanced =
      feature_info_->feature_flags().blend_equation_advanced;
  caps.blend_equation_advanced_coherent =
      feature_info_->feature_flags().blend_equation_advanced_coherent;
  caps.texture_rg = feature_info_->feature_flags().ext_texture_rg;
  return caps;
}
