void RenderViewImpl::SetDeviceScaleFactor(float device_scale_factor) {
  RenderWidget::SetDeviceScaleFactor(device_scale_factor);
  if (webview()) {
    webview()->setDeviceScaleFactor(device_scale_factor);
    webview()->settings()->setAcceleratedCompositingForFixedPositionEnabled(
        ShouldUseFixedPositionCompositing(device_scale_factor_));
  }
  if (auto_resize_mode_)
    AutoResizeCompositor();
}
