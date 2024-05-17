void HTMLMediaElement::forgetResourceSpecificTracks() {
  if (m_textTracks) {
    TrackDisplayUpdateScope scope(this->cueTimeline());
    m_textTracks->removeAllInbandTracks();
  }

  m_audioTracks->removeAll();
  m_videoTracks->removeAll();

  m_audioTracksTimer.stop();
}
