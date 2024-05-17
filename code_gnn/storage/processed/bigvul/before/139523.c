static bool ExecuteUnscript(LocalFrame& frame,
                            Event*,
                            EditorCommandSource source,
                            const String&) {
  return ExecuteApplyStyle(frame, source, InputEvent::InputType::kNone,
                           CSSPropertyVerticalAlign, "baseline");
}
