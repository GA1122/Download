void HTMLMediaElement::configureTextTrackDisplay() {
  DCHECK(m_textTracks);
  BLINK_MEDIA_LOG << "configureTextTrackDisplay(" << (void*)this << ")";

  if (m_processingPreferenceChange)
    return;

  bool haveVisibleTextTrack = m_textTracks->hasShowingTracks();
  m_textTracksVisible = haveVisibleTextTrack;

  if (!haveVisibleTextTrack && !mediaControls())
    return;

  cueTimeline().updateActiveCues(currentTime());

  updateTextTrackDisplay();
}
