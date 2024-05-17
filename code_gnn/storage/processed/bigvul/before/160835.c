void RenderViewImpl::UpdateWebViewWithDeviceScaleFactor() {
  if (!webview())
    return;
  if (IsUseZoomForDSFEnabled()) {
    webview()->SetZoomFactorForDeviceScaleFactor(device_scale_factor_);
  } else {
    webview()->SetDeviceScaleFactor(device_scale_factor_);
  }
  webview()->GetSettings()->SetPreferCompositingToLCDTextEnabled(
      PreferCompositingToLCDText(compositor_deps_, device_scale_factor_));
}
