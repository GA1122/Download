void PageHandler::InnerSwapCompositorFrame() {
  if (!host_)
    return;

  if (frames_in_flight_ > kMaxScreencastFramesInFlight)
    return;

  if (++frame_counter_ % capture_every_nth_frame_)
    return;

  RenderWidgetHostViewBase* const view =
      static_cast<RenderWidgetHostViewBase*>(host_->GetView());
  if (!view || !view->IsSurfaceAvailableForCopy())
    return;

  const gfx::Size surface_size = view->GetCompositorViewportPixelSize();
  if (surface_size.IsEmpty())
    return;

  const gfx::Size snapshot_size = DetermineSnapshotSize(
      surface_size, screencast_max_width_, screencast_max_height_);
  if (snapshot_size.IsEmpty())
    return;

  double top_controls_height =
      last_compositor_frame_metadata_.top_controls_height;
  double top_controls_shown_ratio =
      last_compositor_frame_metadata_.top_controls_shown_ratio;

  std::unique_ptr<Page::ScreencastFrameMetadata> page_metadata =
      BuildScreencastFrameMetadata(
          surface_size, last_compositor_frame_metadata_.device_scale_factor,
          last_compositor_frame_metadata_.page_scale_factor,
          last_compositor_frame_metadata_.root_scroll_offset,
          top_controls_height, top_controls_shown_ratio);
  if (!page_metadata)
    return;

  view->CopyFromSurface(
      gfx::Rect(), snapshot_size,
      base::BindOnce(&PageHandler::ScreencastFrameCaptured,
                     weak_factory_.GetWeakPtr(), std::move(page_metadata)));
  frames_in_flight_++;
}
