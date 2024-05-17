void HTMLMediaElement::setMuted(bool muted) {
  BLINK_MEDIA_LOG << "setMuted(" << (void*)this << ", " << BoolString(muted)
                  << ")";

  if (muted_ == muted)
    return;

  muted_ = muted;

  ScheduleEvent(EventTypeNames::volumechange);

  if (!muted_ && !autoplay_policy_->RequestAutoplayUnmute())
    pause();

  if (GetWebMediaPlayer())
    GetWebMediaPlayer()->SetVolume(EffectiveMediaVolume());

  autoplay_policy_->StopAutoplayMutedWhenVisible();
}
