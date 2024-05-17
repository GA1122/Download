 void HTMLInputElement::DispatchFocusInEvent(
     const AtomicString& event_type,
     Element* old_focused_element,
    WebFocusType type,
    InputDeviceCapabilities* source_capabilities) {
  if (event_type == EventTypeNames::DOMFocusIn)
    input_type_view_->HandleFocusInEvent(old_focused_element, type);
  HTMLFormControlElementWithState::DispatchFocusInEvent(
      event_type, old_focused_element, type, source_capabilities);
 }
