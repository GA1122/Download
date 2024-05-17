static bool ExecuteJustifyRight(LocalFrame& frame,
                                Event*,
                                EditorCommandSource source,
                                const String&) {
  return ExecuteApplyParagraphStyle(frame, source,
                                    InputEvent::InputType::kFormatJustifyRight,
                                    CSSPropertyTextAlign, "right");
}
