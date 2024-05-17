void RenderWidgetHostImpl::NotifyScreenInfoChanged() {
  WasResized();

  if (touch_emulator_) {
    touch_emulator_->SetDeviceScaleFactor(GetScaleFactorForView(view_.get()));
  }
}
