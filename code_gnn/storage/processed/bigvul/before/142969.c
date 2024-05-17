void HTMLMediaElement::setMuted(bool muted) {
  BLINK_MEDIA_LOG << "setMuted(" << (void*)this << ", " << BoolString(muted)
                  << ")";

  if (muted_ == muted)
    return;

  muted_ = muted;

  ScheduleEvent(event_type_names::kVolumechange);

  if (!muted_ && !autoplay_policy_->RequestAutoplayUnmute())
    pause();

  if (!muted_ && PotentiallyPlaying())
    was_always_muted_ = false;

  if (GetWebMediaPlayer())
    GetWebMediaPlayer()->SetVolume(EffectiveMediaVolume());

  autoplay_policy_->StopAutoplayMutedWhenVisible();
}
