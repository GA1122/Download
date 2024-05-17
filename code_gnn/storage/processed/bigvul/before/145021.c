bool CustomButton::OnMousePressed(const ui::MouseEvent& event) {
  if (state_ != STATE_DISABLED) {
    if (ShouldEnterPushedState(event) && HitTestPoint(event.location()))
      SetState(STATE_PRESSED);
    if (request_focus_on_press_)
      RequestFocus();
    if (IsTriggerableEvent(event) && notify_action_ == NOTIFY_ON_PRESS) {
      NotifyClick(event);
    }
  }
  return true;
}
