void MediaControlTextTrackListElement::defaultEventHandler(Event* event) {
  if (event->type() == EventTypeNames::change) {
    Node* target = event->target()->toNode();
    if (!target || !target->isElementNode())
      return;

    mediaControls().disableShowingTextTracks();
    int trackIndex =
        toElement(target)->getIntegralAttribute(trackIndexAttrName());
    if (trackIndex != trackIndexOffValue) {
      DCHECK_GE(trackIndex, 0);
      mediaControls().showTextTrackAtIndex(trackIndex);
      mediaElement().disableAutomaticTextTrackSelection();
    }

    event->setDefaultHandled();
  }
  MediaControlDivElement::defaultEventHandler(event);
}
