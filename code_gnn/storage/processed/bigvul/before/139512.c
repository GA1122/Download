static bool ExecuteSubscript(LocalFrame& frame,
                             Event*,
                             EditorCommandSource source,
                             const String&) {
  return ExecuteToggleStyle(frame, source,
                            InputEvent::InputType::kFormatSubscript,
                            CSSPropertyVerticalAlign, "baseline", "sub");
}
