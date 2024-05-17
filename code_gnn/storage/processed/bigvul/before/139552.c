static TriState StateStyleWithCSS(LocalFrame& frame, Event*) {
  return frame.GetEditor().ShouldStyleWithCSS() ? kTrueTriState
                                                : kFalseTriState;
}
