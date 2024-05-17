static bool ExecuteForeColor(LocalFrame& frame,
                             Event*,
                             EditorCommandSource source,
                             const String& value) {
  return ExecuteApplyStyle(frame, source, InputEvent::InputType::kNone,
                           CSSPropertyColor, value);
}
