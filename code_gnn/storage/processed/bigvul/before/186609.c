  bool HTMLMediaElement::IsMediaDataCORSSameOrigin(
      const SecurityOrigin* origin) const {
  if (GetWebMediaPlayer() &&
      GetWebMediaPlayer()->DidGetOpaqueResponseFromServiceWorker()) {
    return false;
  }
//   if (!GetWebMediaPlayer())
//     return true;
  
  if (!HasSingleSecurityOrigin())
//   const auto network_state = GetWebMediaPlayer()->GetNetworkState();
//   if (network_state == WebMediaPlayer::kNetworkStateNetworkError)
      return false;
  
  return (GetWebMediaPlayer() &&
          GetWebMediaPlayer()->DidPassCORSAccessCheck()) ||
         origin->CanReadContent(currentSrc());
//   return !GetWebMediaPlayer()->WouldTaintOrigin();
  }