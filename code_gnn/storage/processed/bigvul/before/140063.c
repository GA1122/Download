void HTMLMediaElement::remoteRouteAvailabilityChanged(
    WebRemotePlaybackAvailability availability) {
  if (remotePlaybackClient())
    remotePlaybackClient()->availabilityChanged(availability);

  if (mediaControls())
    mediaControls()->onRemotePlaybackAvailabilityChanged();
}
