void RenderViewImpl::SetDeviceScaleFactor(float device_scale_factor) {
  RenderWidget::SetDeviceScaleFactor(device_scale_factor);
  if (webview())
    webview()->setDeviceScaleFactor(device_scale_factor);
}
