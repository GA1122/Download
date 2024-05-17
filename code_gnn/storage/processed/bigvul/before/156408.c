void PageHandler::OnFrameFromVideoConsumer(
    scoped_refptr<media::VideoFrame> frame) {
  if (!host_)
    return;

  RenderWidgetHostViewBase* const view =
      static_cast<RenderWidgetHostViewBase*>(host_->GetView());
  if (!view)
    return;

  const gfx::Size surface_size = view->GetCompositorViewportPixelSize();
  if (surface_size.IsEmpty())
    return;

  if (surface_size != last_surface_size_) {
    last_surface_size_ = surface_size;
    gfx::Size snapshot_size = DetermineSnapshotSize(
        surface_size, screencast_max_width_, screencast_max_height_);
    if (!snapshot_size.IsEmpty())
      video_consumer_->SetMinAndMaxFrameSize(snapshot_size, snapshot_size);
    return;
  }

  double device_scale_factor, page_scale_factor;
  double top_controls_height, top_controls_shown_ratio;
  gfx::Vector2dF root_scroll_offset;
  GetMetadataFromFrame(*frame, &device_scale_factor, &page_scale_factor,
                       &root_scroll_offset, &top_controls_height,
                       &top_controls_shown_ratio);
  std::unique_ptr<Page::ScreencastFrameMetadata> page_metadata =
      BuildScreencastFrameMetadata(
          surface_size, device_scale_factor, page_scale_factor,
          root_scroll_offset, top_controls_height, top_controls_shown_ratio);
  if (!page_metadata)
    return;

  ScreencastFrameCaptured(std::move(page_metadata),
                          DevToolsVideoConsumer::GetSkBitmapFromFrame(frame));
}
