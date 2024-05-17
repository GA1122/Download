void RenderViewImpl::OnDeviceScaleFactorChanged() {
  RenderWidget::OnDeviceScaleFactorChanged();
  UpdateWebViewWithDeviceScaleFactor();
  if (auto_resize_mode_)
    AutoResizeCompositor(viz::LocalSurfaceId());
}
