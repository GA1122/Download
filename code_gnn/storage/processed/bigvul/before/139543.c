static TriState StateItalic(LocalFrame& frame, Event*) {
  return StateStyle(frame, CSSPropertyFontStyle, "italic");
}
