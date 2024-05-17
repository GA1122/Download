static TriState StateStrikethrough(LocalFrame& frame, Event*) {
  return StateStyle(frame, CSSPropertyWebkitTextDecorationsInEffect,
                    "line-through");
}
