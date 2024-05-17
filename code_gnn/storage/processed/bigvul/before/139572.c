static String ValueForeColor(const EditorInternalCommand&,
                             LocalFrame& frame,
                             Event*) {
  return ValueStyle(frame, CSSPropertyColor);
}
