void CustomButton::OnDragDone() {
  if (state_ != STATE_DISABLED)
    SetState(STATE_NORMAL);
}
