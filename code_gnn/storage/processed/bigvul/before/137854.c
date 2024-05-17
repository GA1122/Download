void MediaControlCastButtonElement::defaultEventHandler(Event* event) {
  if (event->type() == EventTypeNames::click) {
    if (m_isOverlayButton)
      Platform::current()->recordAction(
          UserMetricsAction("Media.Controls.CastOverlay"));
    else
      Platform::current()->recordAction(
          UserMetricsAction("Media.Controls.Cast"));

    if (m_isOverlayButton && !m_clickUseCounted) {
      m_clickUseCounted = true;
      recordMetrics(CastOverlayMetrics::Clicked);
    }
    if (mediaElement().isPlayingRemotely()) {
      mediaElement().requestRemotePlaybackControl();
    } else {
      mediaElement().requestRemotePlayback();
    }
  }
  MediaControlInputElement::defaultEventHandler(event);
}
