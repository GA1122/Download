void RenderWidgetHostViewAndroid::UpdateFrameInfo(
    const gfx::Vector2d& scroll_offset,
    float page_scale_factor,
    float min_page_scale_factor,
    float max_page_scale_factor,
    const gfx::Size& content_size) {
  if (content_view_core_) {
    content_view_core_->UpdateContentSize(content_size.width(),
                                          content_size.height());
    content_view_core_->UpdatePageScaleLimits(min_page_scale_factor,
                                              max_page_scale_factor);
    content_view_core_->UpdateScrollOffsetAndPageScaleFactor(scroll_offset.x(),
                                                             scroll_offset.y(),
                                                             page_scale_factor);
  }
}
