void RenderWidgetHostImpl::LostCapture() {
  if (touch_emulator_)
    touch_emulator_->CancelTouch();

  GetWidgetInputHandler()->MouseCaptureLost();

  if (delegate_)
    delegate_->LostCapture(this);
}
