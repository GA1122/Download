bool MediaControlCastButtonElement::keepEventInNode(Event* event) {
  return isUserInteractionEvent(event);
}
