void RenderWidgetHostImpl::SetPageFocus(bool focused) {
  is_focused_ = focused;

  if (!focused) {
    if (IsMouseLocked())
      view_->UnlockMouse();

    if (touch_emulator_)
      touch_emulator_->CancelTouch();
  }

  GetWidgetInputHandler()->SetFocus(focused);

  if (RenderViewHost::From(this) && delegate_)
    delegate_->ReplicatePageFocus(focused);
}
