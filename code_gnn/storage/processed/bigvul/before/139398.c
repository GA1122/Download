static bool ExecuteApplyParagraphStyle(LocalFrame& frame,
                                       EditorCommandSource source,
                                       InputEvent::InputType input_type,
                                       CSSPropertyID property_id,
                                       const String& property_value) {
  MutableStylePropertySet* style =
      MutableStylePropertySet::Create(kHTMLQuirksMode);
  style->SetProperty(property_id, property_value);
  switch (source) {
    case kCommandFromMenuOrKeyBinding:
      frame.GetEditor().ApplyParagraphStyleToSelection(style, input_type);
      return true;
    case kCommandFromDOM:
      frame.GetEditor().ApplyParagraphStyle(style, input_type);
      return true;
  }
  NOTREACHED();
  return false;
}
