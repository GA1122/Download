void RenderViewImpl::OnDisableAutoResize(const gfx::Size& new_size) {
  DCHECK(disable_scrollbars_size_limit_.IsEmpty());
  if (!webview())
    return;
  webview()->disableAutoResizeMode();

  Resize(new_size, resizer_rect_, is_fullscreen_, NO_RESIZE_ACK);
}
