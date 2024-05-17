void RenderWidgetHostViewAura::OnDeviceScaleFactorChanged(
    float device_scale_factor) {
  if (!host_)
    return;

  BackingStoreAura* backing_store = static_cast<BackingStoreAura*>(
      host_->GetBackingStore(false));
  if (backing_store)   
    backing_store->ScaleFactorChanged(device_scale_factor);

  UpdateScreenInfo(window_);

  const gfx::Display display = gfx::Screen::GetScreenFor(window_)->
      GetDisplayNearestWindow(window_);
  DCHECK_EQ(device_scale_factor, display.device_scale_factor());
  current_cursor_.SetDisplayInfo(display);
}
