gpu::Capabilities GLES2DecoderPassthroughImpl::GetCapabilities() {
  DCHECK(initialized());
  Capabilities caps;

  PopulateNumericCapabilities(&caps, feature_info_.get());

  api()->glGetIntegervFn(GL_BIND_GENERATES_RESOURCE_CHROMIUM,
                         &caps.bind_generates_resource_chromium);
  DCHECK_EQ(caps.bind_generates_resource_chromium != GL_FALSE,
            group_->bind_generates_resource());

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
  caps.texture_format_etc1_npot = caps.texture_format_etc1;
  caps.texture_rectangle = feature_info_->feature_flags().arb_texture_rectangle;
  caps.texture_usage = feature_info_->feature_flags().angle_texture_usage;
  caps.texture_storage = feature_info_->feature_flags().ext_texture_storage;
  caps.discard_framebuffer =
      feature_info_->feature_flags().ext_discard_framebuffer;
  caps.sync_query = feature_info_->feature_flags().chromium_sync_query;
#if defined(OS_MACOSX)
  caps.iosurface = true;
#endif
  caps.blend_equation_advanced =
      feature_info_->feature_flags().blend_equation_advanced;
  caps.blend_equation_advanced_coherent =
      feature_info_->feature_flags().blend_equation_advanced_coherent;
  caps.texture_rg = feature_info_->feature_flags().ext_texture_rg;
  caps.texture_norm16 = feature_info_->feature_flags().ext_texture_norm16;
  caps.texture_half_float_linear =
      feature_info_->feature_flags().enable_texture_half_float_linear;
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
      feature_info_->workarounds().max_copy_texture_chromium_size;
  caps.render_buffer_format_bgra8888 =
      feature_info_->feature_flags().ext_render_buffer_format_bgra8888;
  caps.occlusion_query_boolean =
      feature_info_->feature_flags().occlusion_query_boolean;
  caps.timer_queries = feature_info_->feature_flags().ext_disjoint_timer_query;
  caps.gpu_rasterization =
      group_->gpu_feature_info()
          .status_values[GPU_FEATURE_TYPE_GPU_RASTERIZATION] ==
      kGpuFeatureStatusEnabled;
  caps.post_sub_buffer = surface_->SupportsPostSubBuffer();
  caps.surfaceless = !offscreen_ && surface_->IsSurfaceless();
  caps.flips_vertically = !offscreen_ && surface_->FlipsVertically();
  caps.msaa_is_slow = feature_info_->workarounds().msaa_is_slow;
  caps.avoid_stencil_buffers =
      feature_info_->workarounds().avoid_stencil_buffers;
  caps.multisample_compatibility =
      feature_info_->feature_flags().ext_multisample_compatibility;
  caps.dc_layers = !offscreen_ && surface_->SupportsDCLayers();
  caps.commit_overlay_planes = surface_->SupportsCommitOverlayPlanes();
  caps.use_dc_overlays_for_video = surface_->UseOverlaysForVideo();
  caps.protected_video_swap_chain = surface_->SupportsProtectedVideo();
  caps.texture_npot = feature_info_->feature_flags().npot_ok;
  caps.chromium_gpu_fence = feature_info_->feature_flags().chromium_gpu_fence;
  caps.chromium_nonblocking_readback = true;
  caps.num_surface_buffers = surface_->GetBufferCount();
  caps.gpu_memory_buffer_formats =
      feature_info_->feature_flags().gpu_memory_buffer_formats;
  caps.texture_target_exception_list =
      group_->gpu_preferences().texture_target_exception_list;

  return caps;
}
