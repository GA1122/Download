void RenderViewImpl::OnResize(const gfx::Size& new_size,
                              const gfx::Size& physical_backing_size,
                              const gfx::Rect& resizer_rect,
                              bool is_fullscreen) {
  if (webview()) {
    webview()->hidePopups();
    if (send_preferred_size_changes_) {
      webview()->mainFrame()->setCanHaveScrollbars(
          ShouldDisplayScrollbars(new_size.width(), new_size.height()));
    }
    UpdateScrollState(webview()->mainFrame());
  }

  RenderWidget::OnResize(new_size, physical_backing_size, resizer_rect,
                         is_fullscreen);
}
