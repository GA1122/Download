static bool ExecuteBackColor(LocalFrame& frame,
                             Event*,
                             EditorCommandSource source,
                             const String& value) {
  return ExecuteApplyStyle(frame, source, InputEvent::InputType::kNone,
                           CSSPropertyBackgroundColor, value);
}
