uint64_t HTMLMediaElement::webkitAudioDecodedByteCount() const {
  if (!GetWebMediaPlayer())
    return 0;
  return GetWebMediaPlayer()->AudioDecodedByteCount();
}
