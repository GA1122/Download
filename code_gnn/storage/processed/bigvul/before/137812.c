MediaControlCastButtonElement::MediaControlCastButtonElement(
    MediaControls& mediaControls,
    bool isOverlayButton)
    : MediaControlInputElement(mediaControls, MediaCastOnButton),
      m_isOverlayButton(isOverlayButton) {
  if (m_isOverlayButton)
    recordMetrics(CastOverlayMetrics::Created);
  setIsPlayingRemotely(false);
}
