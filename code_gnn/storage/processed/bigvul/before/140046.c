Nullable<ExceptionCode> HTMLMediaElement::play() {
  BLINK_MEDIA_LOG << "play(" << (void*)this << ")";

  if (!UserGestureIndicator::processingUserGesture()) {
    m_autoplayUmaHelper->onAutoplayInitiated(AutoplaySource::Method);
    if (isGestureNeededForPlayback()) {
      if (!m_paused) {
        playInternal();
        return nullptr;
      }

      m_autoplayUmaHelper->recordCrossOriginAutoplayResult(
          CrossOriginAutoplayResult::AutoplayBlocked);
      String message = ExceptionMessages::failedToExecute(
          "play", "HTMLMediaElement",
          "API can only be initiated by a user gesture.");
      document().addConsoleMessage(ConsoleMessage::create(
          JSMessageSource, WarningMessageLevel, message));
      return NotAllowedError;
    } else {
      if (isGestureNeededForPlaybackIfCrossOriginExperimentEnabled()) {
        m_autoplayUmaHelper->recordCrossOriginAutoplayResult(
            CrossOriginAutoplayResult::AutoplayBlocked);
      } else {
        m_autoplayUmaHelper->recordCrossOriginAutoplayResult(
            CrossOriginAutoplayResult::AutoplayAllowed);
      }
    }
  } else {
    m_autoplayUmaHelper->recordCrossOriginAutoplayResult(
        CrossOriginAutoplayResult::PlayedWithGesture);
    UserGestureIndicator::utilizeUserGesture();
    unlockUserGesture();
  }

  if (m_error && m_error->code() == MediaError::kMediaErrSrcNotSupported)
    return NotSupportedError;

  playInternal();

  return nullptr;
}
