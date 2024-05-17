void HTMLFormControlElement::DispatchChangeEvent() {
  DispatchScopedEvent(*Event::CreateBubble(event_type_names::kChange));
}
