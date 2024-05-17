void HTMLFormElement::handleLocalEvents(Event& event) {
  Node* targetNode = event.target()->toNode();
  if (event.eventPhase() != Event::kCapturingPhase && targetNode &&
      targetNode != this && (event.type() == EventTypeNames::submit ||
                             event.type() == EventTypeNames::reset)) {
    event.stopPropagation();
    return;
  }
  HTMLElement::handleLocalEvents(event);
}
