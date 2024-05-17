void MediaControlOverlayPlayButtonElement::defaultEventHandler(Event* event) {
  if (event->type() == EventTypeNames::click && mediaElement().paused()) {
    Platform::current()->recordAction(
        UserMetricsAction("Media.Controls.PlayOverlay"));
    mediaElement().play();
    updateDisplayType();
    event->setDefaultHandled();
  }
}
