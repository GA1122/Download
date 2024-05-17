void RenderView::OnResize(const gfx::Size& new_size,
                          const gfx::Rect& resizer_rect) {
  if (webview()) {
    webview()->hidePopups();
    if (send_preferred_size_changes_) {
      webview()->mainFrame()->setCanHaveScrollbars(
          should_display_scrollbars(new_size.width(), new_size.height()));
    }
  }

  RenderWidget::OnResize(new_size, resizer_rect);
}
