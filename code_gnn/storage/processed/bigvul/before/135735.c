void DispatchBeforeInputFromComposition(EventTarget* target,
                                        InputEvent::InputType input_type,
                                        const String& data) {
  if (!RuntimeEnabledFeatures::InputEventEnabled())
    return;
  if (!target)
    return;
  InputEvent* before_input_event = InputEvent::CreateBeforeInput(
      input_type, data, InputEvent::kNotCancelable,
      InputEvent::EventIsComposing::kIsComposing, nullptr);
  target->DispatchEvent(before_input_event);
}
