static bool ExecuteApplyStyle(LocalFrame& frame,
                              EditorCommandSource source,
                              InputEvent::InputType input_type,
                              CSSPropertyID property_id,
                              CSSValueID property_value) {
  MutableStylePropertySet* style =
      MutableStylePropertySet::Create(kHTMLQuirksMode);
  style->SetProperty(property_id, property_value);
  return ApplyCommandToFrame(frame, source, input_type, style);
}
