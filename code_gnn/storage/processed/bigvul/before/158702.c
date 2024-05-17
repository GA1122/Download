Capabilities GLES2DecoderImpl::GetCapabilities() {
  DCHECK(initialized());
  Capabilities caps;
  const gl::GLVersionInfo& version_info = gl_version_info();
  caps.VisitPrecisions([&version_info](
                           GLenum shader, GLenum type,
                           Capabilities::ShaderPrecision* shader_precision) {
    GLint range[2] = {0, 0};
    GLint precision = 0;
    QueryShaderPrecisionFormat(version_info, shader, type, range, &precision);
    shader_precision->min_range = range[0];
    shader_precision->max_range = range[1];
    shader_precision->precision = precision;
  });
  DoGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
                &caps.max_combined_texture_image_units, 1);
  DoGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE, &caps.max_cube_map_texture_size,
                1);
  DoGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS,
                &caps.max_fragment_uniform_vectors, 1);
  DoGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &caps.max_renderbuffer_size, 1);
  DoGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &caps.max_texture_image_units, 1);
  DoGetIntegerv(GL_MAX_TEXTURE_SIZE, &caps.max_texture_size, 1);
  DoGetIntegerv(GL_MAX_VARYING_VECTORS, &caps.max_varying_vectors, 1);
  DoGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &caps.max_vertex_attribs, 1);
  DoGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
                &caps.max_vertex_texture_image_units, 1);
  DoGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &caps.max_vertex_uniform_vectors,
                1);
  {
    GLint dims[2] = {0, 0};
    DoGetIntegerv(GL_MAX_VIEWPORT_DIMS, dims, 2);
    caps.max_viewport_width = dims[0];
    caps.max_viewport_height = dims[1];
  }
  DoGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS,
                &caps.num_compressed_texture_formats, 1);
  DoGetIntegerv(GL_NUM_SHADER_BINARY_FORMATS, &caps.num_shader_binary_formats,
                1);
  DoGetIntegerv(GL_BIND_GENERATES_RESOURCE_CHROMIUM,
                &caps.bind_generates_resource_chromium, 1);
  if (feature_info_->IsWebGL2OrES3Context()) {
    DoGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &caps.max_3d_texture_size, 1);
    DoGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &caps.max_array_texture_layers,
                  1);
    DoGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &caps.max_color_attachments, 1);
    DoGetInteger64v(GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS,
                    &caps.max_combined_fragment_uniform_components, 1);
    DoGetIntegerv(GL_MAX_COMBINED_UNIFORM_BLOCKS,
                  &caps.max_combined_uniform_blocks, 1);
    DoGetInteger64v(GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS,
                    &caps.max_combined_vertex_uniform_components, 1);
    DoGetIntegerv(GL_MAX_DRAW_BUFFERS, &caps.max_draw_buffers, 1);
    DoGetInteger64v(GL_MAX_ELEMENT_INDEX, &caps.max_element_index, 1);
    DoGetIntegerv(GL_MAX_ELEMENTS_INDICES, &caps.max_elements_indices, 1);
    DoGetIntegerv(GL_MAX_ELEMENTS_VERTICES, &caps.max_elements_vertices, 1);
    DoGetIntegerv(GL_MAX_FRAGMENT_INPUT_COMPONENTS,
                  &caps.max_fragment_input_components, 1);
    DoGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_BLOCKS,
                  &caps.max_fragment_uniform_blocks, 1);
    DoGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
                  &caps.max_fragment_uniform_components, 1);
    DoGetIntegerv(GL_MAX_PROGRAM_TEXEL_OFFSET, &caps.max_program_texel_offset,
                  1);
    DoGetInteger64v(GL_MAX_SERVER_WAIT_TIMEOUT, &caps.max_server_wait_timeout,
                    1);
    if (caps.max_server_wait_timeout < 0)
      caps.max_server_wait_timeout = 0;
    DoGetFloatv(GL_MAX_TEXTURE_LOD_BIAS, &caps.max_texture_lod_bias, 1);
    DoGetIntegerv(GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS,
                  &caps.max_transform_feedback_interleaved_components, 1);
    DoGetIntegerv(GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS,
                  &caps.max_transform_feedback_separate_attribs, 1);
    DoGetIntegerv(GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS,
                  &caps.max_transform_feedback_separate_components, 1);
    DoGetInteger64v(GL_MAX_UNIFORM_BLOCK_SIZE, &caps.max_uniform_block_size, 1);
    DoGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS,
                  &caps.max_uniform_buffer_bindings, 1);
    DoGetIntegerv(GL_MAX_VARYING_COMPONENTS, &caps.max_varying_components, 1);
    DoGetIntegerv(GL_MAX_VERTEX_OUTPUT_COMPONENTS,
                  &caps.max_vertex_output_components, 1);
    DoGetIntegerv(GL_MAX_VERTEX_UNIFORM_BLOCKS, &caps.max_vertex_uniform_blocks,
                  1);
    DoGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS,
                  &caps.max_vertex_uniform_components, 1);
    DoGetIntegerv(GL_MIN_PROGRAM_TEXEL_OFFSET, &caps.min_program_texel_offset,
                  1);
    DoGetIntegerv(GL_NUM_EXTENSIONS, &caps.num_extensions, 1);
    DoGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS,
                  &caps.num_program_binary_formats, 1);
    DoGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT,
                  &caps.uniform_buffer_offset_alignment, 1);
    caps.major_version = 3;
    caps.minor_version = 0;
  }
  if (feature_info_->feature_flags().multisampled_render_to_texture ||
      feature_info_->feature_flags().chromium_framebuffer_multisample ||
      feature_info_->IsWebGL2OrES3Context()) {
    DoGetIntegerv(GL_MAX_SAMPLES, &caps.max_samples, 1);
  }

  caps.num_stencil_bits = num_stencil_bits_;

  caps.egl_image_external =
      feature_info_->feature_flags().oes_egl_image_external;
  caps.texture_format_astc =
      feature_info_->feature_flags().ext_texture_format_astc;
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
  caps.disable_one_component_textures =
      mailbox_manager()->UsesSync() &&
      workarounds().avoid_one_component_egl_images;
  caps.texture_rectangle = feature_info_->feature_flags().arb_texture_rectangle;
  caps.texture_usage = feature_info_->feature_flags().angle_texture_usage;
  caps.texture_storage = feature_info_->feature_flags().ext_texture_storage;
  caps.discard_framebuffer =
      feature_info_->feature_flags().ext_discard_framebuffer;
  caps.sync_query = feature_info_->feature_flags().chromium_sync_query;

  caps.chromium_image_rgb_emulation = ChromiumImageNeedsRGBEmulation();
#if defined(OS_MACOSX)
  caps.iosurface = true;
#endif
  caps.use_gpu_fences_for_overlay_planes = surface_->SupportsPlaneGpuFences();

  caps.post_sub_buffer = supports_post_sub_buffer_;
  caps.swap_buffers_with_bounds = supports_swap_buffers_with_bounds_;
  caps.commit_overlay_planes = supports_commit_overlay_planes_;
  caps.surfaceless = surfaceless_;
  bool is_offscreen = !!offscreen_target_frame_buffer_.get();
  caps.flips_vertically = !is_offscreen && surface_->FlipsVertically();
  caps.msaa_is_slow = workarounds().msaa_is_slow;
  caps.avoid_stencil_buffers = workarounds().avoid_stencil_buffers;
  caps.multisample_compatibility =
      feature_info_->feature_flags().ext_multisample_compatibility;
  caps.dc_layers = supports_dc_layers_;
  caps.use_dc_overlays_for_video = surface_->UseOverlaysForVideo();
  caps.protected_video_swap_chain = surface_->SupportsProtectedVideo();

  caps.blend_equation_advanced =
      feature_info_->feature_flags().blend_equation_advanced;
  caps.blend_equation_advanced_coherent =
      feature_info_->feature_flags().blend_equation_advanced_coherent;
  caps.texture_rg = feature_info_->feature_flags().ext_texture_rg;
  caps.texture_norm16 = feature_info_->feature_flags().ext_texture_norm16;
  caps.texture_half_float_linear =
      feature_info_->oes_texture_half_float_linear_available();
  caps.color_buffer_half_float_rgba =
      feature_info_->ext_color_buffer_float_available() ||
      feature_info_->ext_color_buffer_half_float_available();
  caps.image_ycbcr_422 =
      feature_info_->feature_flags().chromium_image_ycbcr_422;
  caps.image_ycbcr_420v =
      feature_info_->feature_flags().chromium_image_ycbcr_420v;
  caps.image_ycbcr_420v_disabled_for_video_frames =
      group_->gpu_preferences()
          .disable_biplanar_gpu_memory_buffers_for_video_frames;
  caps.image_xr30 = feature_info_->feature_flags().chromium_image_xr30;
  caps.image_xb30 = feature_info_->feature_flags().chromium_image_xb30;
  caps.max_copy_texture_chromium_size =
      workarounds().max_copy_texture_chromium_size;
  caps.render_buffer_format_bgra8888 =
      feature_info_->feature_flags().ext_render_buffer_format_bgra8888;
  caps.occlusion_query = feature_info_->feature_flags().occlusion_query;
  caps.occlusion_query_boolean =
      feature_info_->feature_flags().occlusion_query_boolean;
  caps.timer_queries = query_manager_->GPUTimingAvailable();
  caps.gpu_rasterization =
      group_->gpu_feature_info()
          .status_values[GPU_FEATURE_TYPE_GPU_RASTERIZATION] ==
      kGpuFeatureStatusEnabled;
  if (workarounds().disable_non_empty_post_sub_buffers_for_onscreen_surfaces &&
      !surface_->IsOffscreen()) {
    caps.disable_non_empty_post_sub_buffers = true;
  }
  if (workarounds().broken_egl_image_ref_counting &&
      group_->gpu_preferences().enable_threaded_texture_mailboxes) {
    caps.disable_2d_canvas_copy_on_write = true;
  }
  caps.texture_npot = feature_info_->feature_flags().npot_ok;
  caps.texture_storage_image =
      feature_info_->feature_flags().chromium_texture_storage_image;
  caps.supports_oop_raster = false;
  caps.chromium_gpu_fence = feature_info_->feature_flags().chromium_gpu_fence;
  caps.unpremultiply_and_dither_copy =
      feature_info_->feature_flags().unpremultiply_and_dither_copy;
  caps.texture_target_exception_list =
      group_->gpu_preferences().texture_target_exception_list;
  caps.separate_stencil_ref_mask_writemask =
      feature_info_->feature_flags().separate_stencil_ref_mask_writemask;
  caps.chromium_nonblocking_readback =
      feature_info_->context_type() == CONTEXT_TYPE_WEBGL2;
  caps.num_surface_buffers = surface_->GetBufferCount();
  caps.mesa_framebuffer_flip_y =
      feature_info_->feature_flags().mesa_framebuffer_flip_y;

  return caps;
}
