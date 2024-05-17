bool MediaControlTimelineElement::willRespondToMouseClickEvents() {
  return isConnected() && document().isActive();
}
