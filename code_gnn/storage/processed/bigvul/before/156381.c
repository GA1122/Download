std::unique_ptr<Page::ScreencastFrameMetadata> BuildScreencastFrameMetadata(
    const gfx::Size& surface_size,
    float device_scale_factor,
    float page_scale_factor,
    const gfx::Vector2dF& root_scroll_offset,
    float top_controls_height,
    float top_controls_shown_ratio) {
  if (surface_size.IsEmpty() || device_scale_factor == 0)
    return nullptr;

  const gfx::SizeF content_size_dip =
      gfx::ScaleSize(gfx::SizeF(surface_size), 1 / device_scale_factor);
  float top_offset_dip = top_controls_height * top_controls_shown_ratio;
  gfx::Vector2dF root_scroll_offset_dip = root_scroll_offset;
  if (IsUseZoomForDSFEnabled()) {
    top_offset_dip /= device_scale_factor;
    root_scroll_offset_dip.Scale(1 / device_scale_factor);
  }
  std::unique_ptr<Page::ScreencastFrameMetadata> page_metadata =
      Page::ScreencastFrameMetadata::Create()
          .SetPageScaleFactor(page_scale_factor)
          .SetOffsetTop(top_offset_dip)
          .SetDeviceWidth(content_size_dip.width())
          .SetDeviceHeight(content_size_dip.height())
          .SetScrollOffsetX(root_scroll_offset_dip.x())
          .SetScrollOffsetY(root_scroll_offset_dip.y())
          .SetTimestamp(base::Time::Now().ToDoubleT())
          .Build();
  return page_metadata;
}
