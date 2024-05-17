void CustomButton::OnMouseCaptureLost() {
  if (state_ != STATE_DISABLED && !InDrag())
    SetState(STATE_NORMAL);
}
