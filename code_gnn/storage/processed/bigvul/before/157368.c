WebMediaPlayer::LoadType HTMLMediaElement::GetLoadType() const {
  if (media_source_)
    return WebMediaPlayer::kLoadTypeMediaSource;

  if (src_object_ ||
      (!current_src_.IsNull() && IsMediaStreamURL(current_src_.GetString())))
    return WebMediaPlayer::kLoadTypeMediaStream;

  return WebMediaPlayer::kLoadTypeURL;
}
