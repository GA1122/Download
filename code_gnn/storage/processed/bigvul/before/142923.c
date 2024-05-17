bool HTMLMediaElement::SupportsLoop() const {
  if (GetLoadType() == WebMediaPlayer::kLoadTypeMediaStream)
    return false;

  if (duration() == std::numeric_limits<double>::infinity())
    return false;

  return true;
}
