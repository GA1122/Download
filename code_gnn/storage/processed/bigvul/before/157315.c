void WebMediaPlayerImpl::SetDelegateState(DelegateState new_state,
                                          bool is_idle) {
  DCHECK(delegate_);
  DVLOG(2) << __func__ << "(" << static_cast<int>(new_state) << ", " << is_idle
           << ")";

  bool has_audio = HasAudio() && !client_->WasAlwaysMuted();
  if (delegate_state_ == new_state &&
      (delegate_state_ != DelegateState::PLAYING ||
       delegate_has_audio_ == has_audio)) {
    return;
  }
  delegate_state_ = new_state;
  delegate_has_audio_ = has_audio;

  switch (new_state) {
    case DelegateState::GONE:
      delegate_->PlayerGone(delegate_id_);
      break;
    case DelegateState::PLAYING: {
      if (HasVideo())
        delegate_->DidPlayerSizeChange(delegate_id_, NaturalSize());
      delegate_->DidPlay(
          delegate_id_, HasVideo(), has_audio,
          media::DurationToMediaContentType(GetPipelineMediaDuration()));
      break;
    }
    case DelegateState::PAUSED:
      delegate_->DidPause(delegate_id_);
      break;
  }

  delegate_->SetIdle(delegate_id_, is_idle);
}
