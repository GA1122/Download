void PageHandler::ScreencastFrameEncoded(viz::CompositorFrameMetadata metadata,
                                         const base::Time& timestamp,
                                         const std::string& data) {
  if (metadata.device_scale_factor == 0 || !host_ || data.empty()) {
    --frames_in_flight_;
    return;
  }

  RenderWidgetHostViewBase* view = static_cast<RenderWidgetHostViewBase*>(
      host_->GetView());
  if (!view) {
    --frames_in_flight_;
    return;
  }

  gfx::SizeF screen_size_dip =
      gfx::ScaleSize(gfx::SizeF(view->GetPhysicalBackingSize()),
                     1 / metadata.device_scale_factor);
  float css_to_dip = metadata.page_scale_factor;
  float top_offset_dip =
      metadata.top_controls_height * metadata.top_controls_shown_ratio;
  if (IsUseZoomForDSFEnabled()) {
    css_to_dip /= metadata.device_scale_factor;
    top_offset_dip /= metadata.device_scale_factor;
  }
  std::unique_ptr<Page::ScreencastFrameMetadata> param_metadata =
      Page::ScreencastFrameMetadata::Create()
          .SetPageScaleFactor(css_to_dip)
          .SetOffsetTop(top_offset_dip)
          .SetDeviceWidth(screen_size_dip.width())
          .SetDeviceHeight(screen_size_dip.height())
          .SetScrollOffsetX(metadata.root_scroll_offset.x())
          .SetScrollOffsetY(metadata.root_scroll_offset.y())
          .SetTimestamp(timestamp.ToDoubleT())
          .Build();
  frontend_->ScreencastFrame(data, std::move(param_metadata), session_id_);
}
