WebMediaPlayer::Preload HTMLMediaElement::effectivePreloadType() const {
  if (autoplay() && !isGestureNeededForPlayback())
    return WebMediaPlayer::PreloadAuto;

  WebMediaPlayer::Preload preload = preloadType();
  if (m_ignorePreloadNone && preload == WebMediaPlayer::PreloadNone)
    return WebMediaPlayer::PreloadMetaData;

  return preload;
}
