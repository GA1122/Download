void RenderViewImpl::SetDeviceScaleFactorForTesting(float factor) {
  device_scale_factor_for_testing_ = factor;

  ResizeParams params;
  params.screen_info = screen_info_;
  params.screen_info.device_scale_factor = factor;
  params.new_size = size();
  params.visible_viewport_size = visible_viewport_size_;
  params.physical_backing_size = gfx::ScaleToCeiledSize(size(), factor);
  params.browser_controls_shrink_blink_size = false;
  params.top_controls_height = 0.f;
  params.is_fullscreen_granted = is_fullscreen_granted();
  params.display_mode = display_mode_;
  params.content_source_id = GetContentSourceId();
  OnResize(params);
}
