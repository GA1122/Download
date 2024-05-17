void HTMLMediaElement::pause() {
  BLINK_MEDIA_LOG << "pause(" << (void*)this << ")";

  if (webMediaPlayer() && UserGestureIndicator::utilizeUserGesture())
    webMediaPlayer()->setBufferingStrategy(
        WebMediaPlayer::BufferingStrategy::Aggressive);

  pauseInternal();
}
