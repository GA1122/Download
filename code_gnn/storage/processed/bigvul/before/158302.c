bool RenderWidgetHostImpl::IsWheelScrollInProgress() {
  return is_in_gesture_scroll_[blink::kWebGestureDeviceTouchpad];
}
