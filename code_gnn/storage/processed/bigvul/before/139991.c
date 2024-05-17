HTMLMediaElement::DirectionOfPlayback HTMLMediaElement::getDirectionOfPlayback()
    const {
  return m_playbackRate >= 0 ? Forward : Backward;
}
