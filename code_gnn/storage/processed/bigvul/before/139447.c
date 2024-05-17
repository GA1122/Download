static bool ExecuteMakeTextWritingDirectionRightToLeft(LocalFrame& frame,
                                                       Event*,
                                                       EditorCommandSource,
                                                       const String&) {
  MutableStylePropertySet* style =
      MutableStylePropertySet::Create(kHTMLQuirksMode);
  style->SetProperty(CSSPropertyUnicodeBidi, CSSValueIsolate);
  style->SetProperty(CSSPropertyDirection, CSSValueRtl);
  frame.GetEditor().ApplyStyle(
      style, InputEvent::InputType::kFormatSetBlockTextDirection);
  return true;
}
