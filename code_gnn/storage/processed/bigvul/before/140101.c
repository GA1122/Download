void HTMLMediaElement::setNetworkState(WebMediaPlayer::NetworkState state) {
  BLINK_MEDIA_LOG << "setNetworkState(" << (void*)this << ", "
                  << static_cast<int>(state) << ") - current state is "
                  << static_cast<int>(m_networkState);

  if (state == WebMediaPlayer::NetworkStateEmpty) {
    setNetworkState(kNetworkEmpty);
    return;
  }

  if (state == WebMediaPlayer::NetworkStateFormatError ||
      state == WebMediaPlayer::NetworkStateNetworkError ||
      state == WebMediaPlayer::NetworkStateDecodeError) {
    mediaLoadingFailed(state);
    return;
  }

  if (state == WebMediaPlayer::NetworkStateIdle) {
    if (m_networkState > kNetworkIdle) {
      changeNetworkStateFromLoadingToIdle();
      setShouldDelayLoadEvent(false);
    } else {
      setNetworkState(kNetworkIdle);
    }
  }

  if (state == WebMediaPlayer::NetworkStateLoading) {
    if (m_networkState < kNetworkLoading || m_networkState == kNetworkNoSource)
      startProgressEventTimer();
    setNetworkState(kNetworkLoading);
  }

  if (state == WebMediaPlayer::NetworkStateLoaded) {
    if (m_networkState != kNetworkIdle)
      changeNetworkStateFromLoadingToIdle();
  }
}
