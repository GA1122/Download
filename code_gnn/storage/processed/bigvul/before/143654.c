void RenderWidgetHostImpl::LostCapture() {
  if (touch_emulator_)
    touch_emulator_->CancelTouch();

  Send(new InputMsg_MouseCaptureLost(routing_id_));

  if (delegate_)
    delegate_->LostCapture(this);
}
