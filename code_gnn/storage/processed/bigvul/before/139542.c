static TriState StateBold(LocalFrame& frame, Event*) {
  return StateStyle(frame, CSSPropertyFontWeight, "bold");
}
