void RenderViewImpl::OnDisableAutoResize(const gfx::Size& new_size) {
  DCHECK(disable_scrollbars_size_limit_.IsEmpty());
  if (!webview())
    return;
  auto_resize_mode_ = false;
  webview()->disableAutoResizeMode();

  Resize(new_size, physical_backing_size_, resizer_rect_, is_fullscreen_,
         NO_RESIZE_ACK);
}
