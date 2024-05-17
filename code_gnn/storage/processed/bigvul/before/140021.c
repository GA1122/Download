void HTMLMediaElement::loadInternal() {
  m_textTracksWhenResourceSelectionBegan.clear();
  if (m_textTracks) {
    for (unsigned i = 0; i < m_textTracks->length(); ++i) {
      TextTrack* track = m_textTracks->anonymousIndexedGetter(i);
      if (track->mode() != TextTrack::disabledKeyword())
        m_textTracksWhenResourceSelectionBegan.push_back(track);
    }
  }

  selectMediaResource();
}
