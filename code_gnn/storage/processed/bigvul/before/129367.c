Capabilities GLES2DecoderImpl::GetCapabilities() {
  DCHECK(initialized());

  Capabilities caps;

  caps.fast_npot_mo8_textures =
      feature_info_->workarounds().enable_chromium_fast_npot_mo8_textures;
  caps.egl_image_external =
      feature_info_->feature_flags().oes_egl_image_external;
  caps.texture_format_bgra8888 =
      feature_info_->feature_flags().ext_texture_format_bgra8888;
  caps.texture_format_etc1 =
      feature_info_->feature_flags().oes_compressed_etc1_rgb8_texture;
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
  caps.map_image = !!image_manager();

  return caps;
}
