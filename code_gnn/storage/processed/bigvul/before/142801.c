HTMLMediaElement::DirectionOfPlayback HTMLMediaElement::GetDirectionOfPlayback()
    const {
  return playback_rate_ >= 0 ? kForward : kBackward;
}
