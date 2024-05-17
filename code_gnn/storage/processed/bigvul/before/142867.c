void HTMLMediaElement::RemoteRouteAvailabilityChanged(
    WebRemotePlaybackAvailability availability) {
  if (RemotePlaybackClient() &&
      !RuntimeEnabledFeatures::NewRemotePlaybackPipelineEnabled()) {
    RemotePlaybackClient()->AvailabilityChanged(availability);
  }
}
