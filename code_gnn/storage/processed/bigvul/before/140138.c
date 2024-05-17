void HTMLMediaElement::updatePlayState() {
  bool isPlaying = webMediaPlayer() && !webMediaPlayer()->paused();
  bool shouldBePlaying = potentiallyPlaying();

  BLINK_MEDIA_LOG << "updatePlayState(" << (void*)this
                  << ") - shouldBePlaying = " << boolString(shouldBePlaying)
                  << ", isPlaying = " << boolString(isPlaying);

  if (shouldBePlaying) {
    setDisplayMode(Video);

    if (!isPlaying) {
      webMediaPlayer()->setRate(playbackRate());
      webMediaPlayer()->setVolume(effectiveMediaVolume());
      webMediaPlayer()->play();
    }

    startPlaybackProgressTimer();
    m_playing = true;
  } else {   
    if (isPlaying) {
      webMediaPlayer()->pause();
    }

    m_playbackProgressTimer.stop();
    m_playing = false;
    double time = currentTime();
    if (time > m_lastSeekTime)
      addPlayedRange(m_lastSeekTime, time);
  }

  if (layoutObject())
    layoutObject()->updateFromElement();
}
