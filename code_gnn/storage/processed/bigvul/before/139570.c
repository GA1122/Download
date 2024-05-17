static String ValueFontSize(const EditorInternalCommand&,
                            LocalFrame& frame,
                            Event*) {
  return ValueStyle(frame, CSSPropertyFontSize);
}
