blink::WebLocalizedString::Name GetSwitchToLocalMessage(
    MediaObserverClient::ReasonToSwitchToLocal reason) {
  switch (reason) {
    case MediaObserverClient::ReasonToSwitchToLocal::NORMAL:
      return blink::WebLocalizedString::kMediaRemotingStopText;
    case MediaObserverClient::ReasonToSwitchToLocal::POOR_PLAYBACK_QUALITY:
      return blink::WebLocalizedString::kMediaRemotingStopByPlaybackQualityText;
    case MediaObserverClient::ReasonToSwitchToLocal::PIPELINE_ERROR:
      return blink::WebLocalizedString::kMediaRemotingStopByErrorText;
    case MediaObserverClient::ReasonToSwitchToLocal::ROUTE_TERMINATED:
      return blink::WebLocalizedString::kMediaRemotingStopNoText;
  }
  NOTREACHED();
  return blink::WebLocalizedString::kMediaRemotingStopNoText;
}
