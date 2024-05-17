void MediaControlPlayButtonElement::defaultEventHandler(Event* event) {
  if (event->type() == EventTypeNames::click) {
    if (mediaElement().paused())
      Platform::current()->recordAction(
          UserMetricsAction("Media.Controls.Play"));
    else
      Platform::current()->recordAction(
          UserMetricsAction("Media.Controls.Pause"));

    const String& url = mediaElement().currentSrc().getString();
    if (mediaElement().error() && !HTMLMediaElement::isMediaStreamURL(url) &&
        !HTMLMediaSource::lookup(url))
      mediaElement().load();

    mediaElement().togglePlayState();
    updateDisplayType();
    event->setDefaultHandled();
  }
  MediaControlInputElement::defaultEventHandler(event);
}
