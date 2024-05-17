bool RenderViewImpl::GetPpapiPluginCaretBounds(gfx::Rect* rect) {
  if (!pepper_delegate_.IsPluginFocused())
    return false;
  *rect = pepper_delegate_.GetCaretBounds();
  return true;
}
