static TriState StateJustifyFull(LocalFrame& frame, Event*) {
  return StateStyle(frame, CSSPropertyTextAlign, "justify");
}
