void RenderViewImpl::OnDisableAutoResize(const gfx::Size& new_size) {
  DCHECK(disable_scrollbars_size_limit_.IsEmpty());
  if (!webview())
    return;
  auto_resize_mode_ = false;
  webview()->DisableAutoResizeMode();

  if (!new_size.IsEmpty()) {
    ResizeParams resize_params;
    resize_params.screen_info = screen_info_;
    resize_params.new_size = new_size;
    resize_params.physical_backing_size = physical_backing_size_;
    resize_params.browser_controls_shrink_blink_size =
        browser_controls_shrink_blink_size_;
    resize_params.top_controls_height = top_controls_height_;
    resize_params.visible_viewport_size = visible_viewport_size_;
    resize_params.is_fullscreen_granted = is_fullscreen_granted();
    resize_params.display_mode = display_mode_;
    resize_params.needs_resize_ack = false;
    Resize(resize_params);
  }
}
