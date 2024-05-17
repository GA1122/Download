bool HTMLMediaElement::IsMediaDataCorsSameOrigin() const {
  if (!GetWebMediaPlayer())
    return true;

  const auto network_state = GetWebMediaPlayer()->GetNetworkState();
  if (network_state == WebMediaPlayer::kNetworkStateNetworkError)
    return false;

  return !GetWebMediaPlayer()->WouldTaintOrigin();
}
