void dispatchInputEventEditableContentChanged(
    Element* startRoot,
    Element* endRoot,
    InputEvent::InputType inputType,
    const String& data,
    InputEvent::EventIsComposing isComposing) {
  if (startRoot)
    dispatchInputEvent(startRoot, inputType, data, isComposing);
  if (endRoot && endRoot != startRoot)
    dispatchInputEvent(endRoot, inputType, data, isComposing);
}
