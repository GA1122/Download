void RenderViewImpl::OnSelectRange(const gfx::Point& start,
                                   const gfx::Point& end) {
  if (!webview())
    return;

  handling_select_range_ = true;
  webview()->focusedFrame()->selectRange(start, end);
  handling_select_range_ = false;
}
