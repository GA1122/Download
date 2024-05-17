void GestureSequence::BreakRailScroll(const TouchEvent& event,
    GesturePoint& point, Gestures* gestures) {
  DCHECK(state_ == GS_SCROLL);
  if (scroll_type_ == ST_HORIZONTAL &&
      point.BreaksHorizontalRail())
    scroll_type_ = ST_FREE;
  else if (scroll_type_ == ST_VERTICAL &&
           point.BreaksVerticalRail())
    scroll_type_ = ST_FREE;
}
