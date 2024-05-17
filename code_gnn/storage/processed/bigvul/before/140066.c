void HTMLMediaElement::removeTextTrack(WebInbandTextTrack* webTrack) {
  if (!m_textTracks)
    return;

  InbandTextTrack* textTrack = toInbandTextTrack(webTrack->client());
  if (!textTrack)
    return;

  m_textTracks->remove(textTrack);
}
