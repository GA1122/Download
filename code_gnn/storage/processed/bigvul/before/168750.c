TouchEmulator* RenderWidgetHostImpl::GetTouchEmulator() {
  if (!touch_emulator_) {
    touch_emulator_.reset(
        new TouchEmulator(this, GetScaleFactorForView(view_.get())));
  }
  return touch_emulator_.get();
}
