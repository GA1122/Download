void HTMLMediaElement::mediaLoadingFailed(WebMediaPlayer::NetworkState error) {
  stopPeriodicTimers();

  if (m_readyState < kHaveMetadata && m_loadState == LoadingFromSourceElement) {
    if (m_currentSourceNode)
      m_currentSourceNode->scheduleErrorEvent();
    else
      BLINK_MEDIA_LOG << "mediaLoadingFailed(" << (void*)this
                      << ") - error event not sent, <source> was removed";


    forgetResourceSpecificTracks();

    if (havePotentialSourceChild()) {
      BLINK_MEDIA_LOG << "mediaLoadingFailed(" << (void*)this
                      << ") - scheduling next <source>";
      scheduleNextSourceChild();
    } else {
      BLINK_MEDIA_LOG << "mediaLoadingFailed(" << (void*)this
                      << ") - no more <source> elements, waiting";
      waitForSourceChange();
    }

    return;
  }

  if (error == WebMediaPlayer::NetworkStateNetworkError &&
      m_readyState >= kHaveMetadata) {
    mediaEngineError(MediaError::create(MediaError::kMediaErrNetwork));
  } else if (error == WebMediaPlayer::NetworkStateDecodeError) {
    mediaEngineError(MediaError::create(MediaError::kMediaErrDecode));
  } else if ((error == WebMediaPlayer::NetworkStateFormatError ||
              error == WebMediaPlayer::NetworkStateNetworkError) &&
             m_loadState == LoadingFromSrcAttr) {
    noneSupported();
  }

  updateDisplayState();
}
