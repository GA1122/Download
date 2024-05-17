void CustomButton::VisibilityChanged(View* starting_from, bool visible) {
  if (state_ == STATE_DISABLED)
    return;
  SetState(visible && ShouldEnterHoveredState() ? STATE_HOVERED : STATE_NORMAL);
}
