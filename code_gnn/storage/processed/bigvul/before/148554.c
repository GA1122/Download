void WebContentsImpl::SetAudioMuted(bool mute) {
  DVLOG(1) << "SetAudioMuted(mute=" << mute << "), was " << IsAudioMuted()
           << " for WebContentsImpl@" << this;

  if (mute == IsAudioMuted())
    return;

  if (mute) {
    if (!audio_muter_)
      audio_muter_.reset(new WebContentsAudioMuter(this));
    audio_muter_->StartMuting();
  } else {
    DCHECK(audio_muter_);
    audio_muter_->StopMuting();
  }

  for (auto& observer : observers_)
    observer.DidUpdateAudioMutingState(mute);

  OnAudioStateChanged(!mute && audio_stream_monitor_.IsCurrentlyAudible());
}
