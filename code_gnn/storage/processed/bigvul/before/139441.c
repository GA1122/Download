static bool ExecuteJustifyCenter(LocalFrame& frame,
                                 Event*,
                                 EditorCommandSource source,
                                 const String&) {
  return ExecuteApplyParagraphStyle(frame, source,
                                    InputEvent::InputType::kFormatJustifyCenter,
                                    CSSPropertyTextAlign, "center");
}
