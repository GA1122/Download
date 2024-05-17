void MediaControlFullscreenButtonElement::defaultEventHandler(Event* event) {
  if (event->type() == EventTypeNames::click) {
    if (mediaElement().isFullscreen()) {
      Platform::current()->recordAction(
          UserMetricsAction("Media.Controls.ExitFullscreen"));
      mediaControls().exitFullscreen();
    } else {
      Platform::current()->recordAction(
          UserMetricsAction("Media.Controls.EnterFullscreen"));
      mediaControls().enterFullscreen();
    }
    event->setDefaultHandled();
  }
  MediaControlInputElement::defaultEventHandler(event);
}
