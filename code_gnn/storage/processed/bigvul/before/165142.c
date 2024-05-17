void HTMLFormElement::HandleLocalEvents(Event& event) {
  Node* target_node = event.target()->ToNode();
  if (event.eventPhase() != Event::kCapturingPhase && target_node &&
      target_node != this &&
      (event.type() == EventTypeNames::submit ||
       event.type() == EventTypeNames::reset)) {
    event.stopPropagation();
    return;
  }
  HTMLElement::HandleLocalEvents(event);
}
