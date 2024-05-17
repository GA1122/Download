bool CustomButton::OnMouseDragged(const ui::MouseEvent& event) {
  if (state_ != STATE_DISABLED) {
    if (HitTestPoint(event.location()))
      SetState(ShouldEnterPushedState(event) ? STATE_PRESSED : STATE_HOVERED);
    else
      SetState(STATE_NORMAL);
  }
  return true;
}
