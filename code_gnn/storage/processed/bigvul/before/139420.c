static bool ExecuteFontSize(LocalFrame& frame,
                            Event*,
                            EditorCommandSource source,
                            const String& value) {
  CSSValueID size;
  if (!HTMLFontElement::CssValueFromFontSizeNumber(value, size))
    return false;
  return ExecuteApplyStyle(frame, source, InputEvent::InputType::kNone,
                           CSSPropertyFontSize, size);
}
