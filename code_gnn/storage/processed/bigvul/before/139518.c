static bool ExecuteToggleStyle(LocalFrame& frame,
                               EditorCommandSource source,
                               InputEvent::InputType input_type,
                               CSSPropertyID property_id,
                               const char* off_value,
                               const char* on_value) {

  bool style_is_present;
  if (frame.GetEditor().Behavior().ShouldToggleStyleBasedOnStartOfSelection())
    style_is_present =
        frame.GetEditor().SelectionStartHasStyle(property_id, on_value);
  else
    style_is_present = frame.GetEditor().SelectionHasStyle(
                           property_id, on_value) == kTrueTriState;

  EditingStyle* style = EditingStyle::Create(
      property_id, style_is_present ? off_value : on_value);
  return ApplyCommandToFrame(frame, source, input_type, style->Style());
}
