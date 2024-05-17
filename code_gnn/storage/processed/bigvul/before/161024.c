float ChromeClientImpl::WindowToViewportScalar(const float scalar_value) const {
  if (!web_view_->Client())
    return scalar_value;
  WebFloatRect viewport_rect(0, 0, scalar_value, 0);
  web_view_->Client()->ConvertWindowToViewport(&viewport_rect);
  return viewport_rect.width;
}
