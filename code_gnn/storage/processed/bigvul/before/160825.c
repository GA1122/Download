void RenderViewImpl::SetDeviceColorSpaceForTesting(
    const gfx::ColorSpace& color_space) {
  ResizeParams params;
  params.screen_info = screen_info_;
  params.screen_info.color_space = color_space;
  params.new_size = size();
  params.visible_viewport_size = visible_viewport_size_;
  params.physical_backing_size = physical_backing_size_;
  params.browser_controls_shrink_blink_size = false;
  params.top_controls_height = 0.f;
  params.is_fullscreen_granted = is_fullscreen_granted();
  params.display_mode = display_mode_;
  OnResize(params);
}
