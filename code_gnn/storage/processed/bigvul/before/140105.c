void HTMLMediaElement::setPlayerPreload() {
  if (m_webMediaPlayer)
    m_webMediaPlayer->setPreload(effectivePreloadType());

  if (loadIsDeferred() && effectivePreloadType() != WebMediaPlayer::PreloadNone)
    startDeferredLoad();
}
