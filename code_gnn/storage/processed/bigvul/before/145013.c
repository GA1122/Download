void CustomButton::OnEnabledChanged() {
  if (enabled() ? (state_ != STATE_DISABLED) : (state_ == STATE_DISABLED))
    return;

  if (enabled())
    SetState(ShouldEnterHoveredState() ? STATE_HOVERED : STATE_NORMAL);
  else
    SetState(STATE_DISABLED);
}
