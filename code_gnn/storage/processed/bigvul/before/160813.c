void RenderViewImpl::OnSetLocalSurfaceIdForAutoResize(
    uint64_t sequence_number,
    const gfx::Size& min_size,
    const gfx::Size& max_size,
    const content::ScreenInfo& screen_info,
    uint32_t content_source_id,
    const viz::LocalSurfaceId& local_surface_id) {
  if (!auto_resize_mode_ || resize_or_repaint_ack_num_ != sequence_number)
    return;

  SetLocalSurfaceIdForAutoResize(sequence_number, screen_info,
                                 content_source_id, local_surface_id);

  if (IsUseZoomForDSFEnabled()) {
    webview()->EnableAutoResizeMode(
        gfx::ScaleToCeiledSize(min_size, device_scale_factor_),
        gfx::ScaleToCeiledSize(max_size, device_scale_factor_));
  } else {
    webview()->EnableAutoResizeMode(min_size, max_size);
  }
}
