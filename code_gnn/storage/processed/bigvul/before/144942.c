void RenderWidgetHostViewAura::OnDeviceScaleFactorChanged(
    float device_scale_factor) {
  if (!window_->GetRootWindow())
    return;

  RenderWidgetHostImpl* host =
      RenderWidgetHostImpl::From(GetRenderWidgetHost());
  if (host && host->delegate())
    host->delegate()->UpdateDeviceScaleFactor(device_scale_factor);

  device_scale_factor_ = device_scale_factor;
  const display::Display display =
      display::Screen::GetScreen()->GetDisplayNearestWindow(window_);
  DCHECK_EQ(device_scale_factor, display.device_scale_factor());
  current_cursor_.SetDisplayInfo(display);
  SnapToPhysicalPixelBoundary();
}
