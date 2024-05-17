unsigned HTMLMediaElement::webkitVideoDecodedByteCount() const {
  if (!webMediaPlayer())
    return 0;
  return webMediaPlayer()->videoDecodedByteCount();
}
