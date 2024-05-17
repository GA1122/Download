WebMediaPlayer::Preload HTMLMediaElement::EffectivePreloadType() const {
  if (Autoplay() && !autoplay_policy_->IsGestureNeededForPlayback())
    return WebMediaPlayer::kPreloadAuto;

  WebMediaPlayer::Preload preload = PreloadType();
  if (ignore_preload_none_ && preload == WebMediaPlayer::kPreloadNone)
    return WebMediaPlayer::kPreloadMetaData;

  return preload;
}
