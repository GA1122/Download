void CustomButton::OnMouseReleased(const ui::MouseEvent& event) {
  if (state_ == STATE_DISABLED)
    return;

  if (!HitTestPoint(event.location())) {
    SetState(STATE_NORMAL);
    return;
  }

  SetState(STATE_HOVERED);
  if (IsTriggerableEvent(event) && notify_action_ == NOTIFY_ON_RELEASE) {
    NotifyClick(event);
  }
}
