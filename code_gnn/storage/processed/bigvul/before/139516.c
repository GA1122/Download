static bool ExecuteToggleItalic(LocalFrame& frame,
                                Event*,
                                EditorCommandSource source,
                                const String&) {
  return ExecuteToggleStyle(frame, source, InputEvent::InputType::kFormatItalic,
                            CSSPropertyFontStyle, "normal", "italic");
}
