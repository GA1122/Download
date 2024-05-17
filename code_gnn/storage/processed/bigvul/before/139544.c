static TriState StateJustifyCenter(LocalFrame& frame, Event*) {
  return StateStyle(frame, CSSPropertyTextAlign, "center");
}
