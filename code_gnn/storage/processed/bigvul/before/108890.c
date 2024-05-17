void RenderViewImpl::GetSelectionBounds(gfx::Rect* start, gfx::Rect* end) {
  if (pepper_delegate_.IsPluginFocused()) {
    gfx::Rect caret = pepper_delegate_.GetCaretBounds();
    *start = caret;
    *end = caret;
    return;
  }
  RenderWidget::GetSelectionBounds(start, end);
}
