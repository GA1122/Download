void HTMLFormControlElement::dispatchChangeEvent() {
  dispatchScopedEvent(Event::createBubble(EventTypeNames::change));
}
