WebMediaPlayer::LoadType HTMLMediaElement::GetLoadType() const {
  if (media_source_)
    return WebMediaPlayer::kLoadTypeMediaSource;

  if (src_object_)
    return WebMediaPlayer::kLoadTypeMediaStream;

  return WebMediaPlayer::kLoadTypeURL;
}
