static bool ExecuteApplyStyle(LocalFrame& frame,
                              EditorCommandSource source,
                              InputEvent::InputType input_type,
                              CSSPropertyID property_id,
                              const String& property_value) {
  MutableStylePropertySet* style =
      MutableStylePropertySet::Create(kHTMLQuirksMode);
  style->SetProperty(property_id, property_value);
  return ApplyCommandToFrame(frame, source, input_type, style);
}
