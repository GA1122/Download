void RenderWidgetHostImpl::Blur() {
  is_focused_ = false;

  if (IsMouseLocked())
    view_->UnlockMouse();

  if (touch_emulator_)
    touch_emulator_->CancelTouch();

  Send(new InputMsg_SetFocus(routing_id_, false));

  if (RenderViewHost::From(this) && delegate_)
    delegate_->ReplicatePageFocus(false);
}
