void DispatchInputEventEditableContentChanged(
    Element* start_root,
    Element* end_root,
    InputEvent::InputType input_type,
    const String& data,
    InputEvent::EventIsComposing is_composing) {
  if (start_root)
    DispatchInputEvent(start_root, input_type, data, is_composing);
  if (end_root && end_root != start_root)
    DispatchInputEvent(end_root, input_type, data, is_composing);
}
