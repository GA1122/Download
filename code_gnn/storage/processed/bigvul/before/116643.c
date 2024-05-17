void RenderViewImpl::OnResize(const gfx::Size& new_size,
                              const gfx::Rect& resizer_rect,
                              bool is_fullscreen) {
  if (webview()) {
    if (webkit_preferences_.default_device_scale_factor)
      webview()->settings()->setLayoutFallbackWidth(
          new_size.width() / webkit_preferences_.default_device_scale_factor);
    webview()->hidePopups();
    if (send_preferred_size_changes_) {
      webview()->mainFrame()->setCanHaveScrollbars(
          ShouldDisplayScrollbars(new_size.width(), new_size.height()));
    }
    UpdateScrollState(webview()->mainFrame());
  }

  RenderWidget::OnResize(new_size, resizer_rect, is_fullscreen);
}