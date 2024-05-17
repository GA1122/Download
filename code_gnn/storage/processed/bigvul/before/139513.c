static bool ExecuteSuperscript(LocalFrame& frame,
                               Event*,
                               EditorCommandSource source,
                               const String&) {
  return ExecuteToggleStyle(frame, source,
                            InputEvent::InputType::kFormatSuperscript,
                            CSSPropertyVerticalAlign, "baseline", "super");
}
