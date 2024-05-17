static TriState StateUnderline(LocalFrame& frame, Event*) {
  return StateStyle(frame, CSSPropertyWebkitTextDecorationsInEffect,
                    "underline");
}
