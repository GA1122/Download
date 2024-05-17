String HTMLMediaElement::preload() const {
  if (GetLoadType() == WebMediaPlayer::kLoadTypeMediaStream)
    return PreloadTypeToString(WebMediaPlayer::kPreloadNone);
  return PreloadTypeToString(PreloadType());
}
