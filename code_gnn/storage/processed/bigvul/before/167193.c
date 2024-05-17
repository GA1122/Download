unsigned HTMLMediaElement::webkitAudioDecodedByteCount() const {
  if (!GetWebMediaPlayer())
    return 0;
  return GetWebMediaPlayer()->AudioDecodedByteCount();
}
