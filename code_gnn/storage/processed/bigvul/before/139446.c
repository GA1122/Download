static bool ExecuteMakeTextWritingDirectionNatural(LocalFrame& frame,
                                                   Event*,
                                                   EditorCommandSource,
                                                   const String&) {
  MutableStylePropertySet* style =
      MutableStylePropertySet::Create(kHTMLQuirksMode);
  style->SetProperty(CSSPropertyUnicodeBidi, CSSValueNormal);
  frame.GetEditor().ApplyStyle(
      style, InputEvent::InputType::kFormatSetBlockTextDirection);
  return true;
}
