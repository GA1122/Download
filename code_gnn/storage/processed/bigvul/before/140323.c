void dispatchInputEvent(Element* target,
                        InputEvent::InputType inputType,
                        const String& data,
                        InputEvent::EventIsComposing isComposing) {
  if (!RuntimeEnabledFeatures::inputEventEnabled())
    return;
  if (!target)
    return;
  InputEvent* inputEvent =
      InputEvent::createInput(inputType, data, isComposing, nullptr);
  target->dispatchScopedEvent(inputEvent);
}
