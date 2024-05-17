void OverscrollControllerAndroid::OnFrameMetadataUpdated(
    const cc::CompositorFrameMetadata& frame_metadata) {
  if (!refresh_effect_ && !glow_effect_)
    return;

  const float scale_factor =
      frame_metadata.page_scale_factor * frame_metadata.device_scale_factor;
  gfx::SizeF viewport_size =
      gfx::ScaleSize(frame_metadata.scrollable_viewport_size, scale_factor);
  gfx::SizeF content_size =
      gfx::ScaleSize(frame_metadata.root_layer_size, scale_factor);
  gfx::Vector2dF content_scroll_offset =
      gfx::ScaleVector2d(frame_metadata.root_scroll_offset, scale_factor);

  if (refresh_effect_) {
    refresh_effect_->OnFrameUpdated(content_scroll_offset,
                                    frame_metadata.root_overflow_y_hidden);
  }

  if (glow_effect_) {
    glow_effect_->OnFrameUpdated(viewport_size, content_size,
                                 content_scroll_offset);
  }
}
