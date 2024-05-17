bool HTMLMediaElement::ended() const {
  return EndedPlayback() && GetDirectionOfPlayback() == kForward;
}
