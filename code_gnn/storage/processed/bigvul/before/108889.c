gfx::Point RenderViewImpl::GetScrollOffset() {
  WebSize scroll_offset = webview()->mainFrame()->scrollOffset();
  return gfx::Point(scroll_offset.width, scroll_offset.height);
}
