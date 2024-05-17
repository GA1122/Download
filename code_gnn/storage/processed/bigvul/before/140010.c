bool HTMLMediaElement::isGestureNeededForPlaybackIfPendingUserGestureIsLocked()
    const {
  if (loadType() == WebMediaPlayer::LoadTypeMediaStream)
    return false;

  if (isHTMLVideoElement() && muted() &&
      RuntimeEnabledFeatures::autoplayMutedVideosEnabled() &&
      !(document().settings() &&
        document().settings()->getDataSaverEnabled()) &&
      !(document().settings() &&
        document().settings()->getForcePreloadNoneForMediaElements()) &&
      isAutoplayAllowedPerSettings()) {
    return false;
  }

  return true;
}
