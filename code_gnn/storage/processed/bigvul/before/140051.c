void HTMLMediaElement::playbackStateChanged() {
  BLINK_MEDIA_LOG << "playbackStateChanged(" << (void*)this << ")";

  if (!webMediaPlayer())
    return;

  if (webMediaPlayer()->paused())
    pauseInternal();
  else
    playInternal();
}
