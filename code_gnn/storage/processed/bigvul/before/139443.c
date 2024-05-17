static bool ExecuteJustifyLeft(LocalFrame& frame,
                               Event*,
                               EditorCommandSource source,
                               const String&) {
  return ExecuteApplyParagraphStyle(frame, source,
                                    InputEvent::InputType::kFormatJustifyLeft,
                                    CSSPropertyTextAlign, "left");
}
