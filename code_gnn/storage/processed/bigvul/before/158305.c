void RenderWidgetHostImpl::LostCapture() {
  if (auto* touch_emulator = GetExistingTouchEmulator())
    touch_emulator->CancelTouch();

  GetWidgetInputHandler()->MouseCaptureLost();

  if (delegate_)
    delegate_->LostCapture(this);
}
