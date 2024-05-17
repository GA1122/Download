bool MediaControlTimelineElement::keepEventInNode(Event* event) {
  return isUserInteractionEventForSlider(event, layoutObject());
}
