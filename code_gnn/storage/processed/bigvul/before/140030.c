void HTMLMediaElement::markCaptionAndSubtitleTracksAsUnconfigured() {
  if (!m_textTracks)
    return;

  for (unsigned i = 0; i < m_textTracks->length(); ++i) {
    TextTrack* textTrack = m_textTracks->anonymousIndexedGetter(i);
    if (textTrack->isVisualKind())
      textTrack->setHasBeenConfigured(false);
  }
}
