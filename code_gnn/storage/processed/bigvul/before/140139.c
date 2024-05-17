void HTMLMediaElement::updatePlaybackRate() {
  if (m_webMediaPlayer && potentiallyPlaying())
    webMediaPlayer()->setRate(playbackRate());
}
