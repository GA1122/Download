void HTMLMediaElement::onVisibilityChangedForAutoplay(bool isVisible) {
  if (!isVisible)
    return;

  if (shouldAutoplay()) {
    m_paused = false;
    scheduleEvent(EventTypeNames::play);
    scheduleNotifyPlaying();
    m_autoplaying = false;

    updatePlayState();
  }

  m_autoplayVisibilityObserver->stop();
  m_autoplayVisibilityObserver = nullptr;
}
