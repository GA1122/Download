unsigned HTMLMediaElement::webkitAudioDecodedByteCount() const {
  if (!webMediaPlayer())
    return 0;
  return webMediaPlayer()->audioDecodedByteCount();
}
