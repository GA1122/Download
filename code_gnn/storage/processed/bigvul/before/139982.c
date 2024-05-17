bool HTMLMediaElement::ended() const {
  return endedPlayback() && getDirectionOfPlayback() == Forward;
}
