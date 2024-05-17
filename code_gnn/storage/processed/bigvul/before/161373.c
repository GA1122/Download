void PageHandler::InnerSwapCompositorFrame() {
  if (!host_ || !host_->GetView())
    return;

  if (frames_in_flight_ > kMaxScreencastFramesInFlight)
    return;

  if (++frame_counter_ % capture_every_nth_frame_)
    return;

  RenderWidgetHostViewBase* view = static_cast<RenderWidgetHostViewBase*>(
      host_->GetView());
  viz::CompositorFrameMetadata& metadata = last_compositor_frame_metadata_;

  float css_to_dip = metadata.page_scale_factor;
  if (IsUseZoomForDSFEnabled())
    css_to_dip /= metadata.device_scale_factor;
  gfx::SizeF viewport_size_dip =
      gfx::ScaleSize(metadata.scrollable_viewport_size, css_to_dip);
  gfx::SizeF screen_size_dip =
      gfx::ScaleSize(gfx::SizeF(view->GetPhysicalBackingSize()),
                     1 / metadata.device_scale_factor);

  content::ScreenInfo screen_info;
  GetWebContents()->GetView()->GetScreenInfo(&screen_info);
  double device_scale_factor = screen_info.device_scale_factor;
  double scale = 1;

  if (screencast_max_width_ > 0) {
    double max_width_dip = screencast_max_width_ / device_scale_factor;
    scale = std::min(scale, max_width_dip / screen_size_dip.width());
  }
  if (screencast_max_height_ > 0) {
    double max_height_dip = screencast_max_height_ / device_scale_factor;
    scale = std::min(scale, max_height_dip / screen_size_dip.height());
  }

  if (scale <= 0)
    scale = 0.1;

  gfx::Size snapshot_size_dip(gfx::ToRoundedSize(
      gfx::ScaleSize(viewport_size_dip, scale)));

  if (snapshot_size_dip.width() > 0 && snapshot_size_dip.height() > 0) {
    view->CopyFromSurface(
        gfx::Rect(), snapshot_size_dip,
        base::Bind(&PageHandler::ScreencastFrameCaptured,
                   weak_factory_.GetWeakPtr(),
                   base::Passed(last_compositor_frame_metadata_.Clone())),
        kN32_SkColorType);
    frames_in_flight_++;
  }
}
