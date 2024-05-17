void MediaControlMuteButtonElement::defaultEventHandler(Event* event) {
  if (event->type() == EventTypeNames::click) {
    if (mediaElement().muted())
      Platform::current()->recordAction(
          UserMetricsAction("Media.Controls.Unmute"));
    else
      Platform::current()->recordAction(
          UserMetricsAction("Media.Controls.Mute"));

    mediaElement().setMuted(!mediaElement().muted());
    event->setDefaultHandled();
  }

  MediaControlInputElement::defaultEventHandler(event);
}
