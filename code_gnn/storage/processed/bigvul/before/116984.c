bool GesturePoint::IsInFlickWindow(const TouchEvent& event) {
  return IsOverMinFlickSpeed() && event.type() != ui::ET_TOUCH_CANCELLED;
}
