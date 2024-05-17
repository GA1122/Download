void HTMLMediaElement::pauseInternal() {
  BLINK_MEDIA_LOG << "pauseInternal(" << (void*)this << ")";

  if (m_networkState == kNetworkEmpty)
    invokeResourceSelectionAlgorithm();

  m_autoplaying = false;

  if (!m_paused) {
    m_paused = true;
    scheduleTimeupdateEvent(false);
    scheduleEvent(EventTypeNames::pause);

    setOfficialPlaybackPosition(currentPlaybackPosition());

    scheduleRejectPlayPromises(AbortError);
  }

  updatePlayState();
}
