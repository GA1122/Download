TextTrackList* HTMLMediaElement::textTracks() {
  if (!m_textTracks)
    m_textTracks = TextTrackList::create(this);

  return m_textTracks.get();
}
