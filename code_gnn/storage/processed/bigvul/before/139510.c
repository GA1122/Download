static bool ExecuteStrikethrough(LocalFrame& frame,
                                 Event*,
                                 EditorCommandSource source,
                                 const String&) {
  CSSIdentifierValue* line_through =
      CSSIdentifierValue::Create(CSSValueLineThrough);
  return ExecuteToggleStyleInList(
      frame, source, InputEvent::InputType::kFormatStrikeThrough,
      CSSPropertyWebkitTextDecorationsInEffect, line_through);
}
