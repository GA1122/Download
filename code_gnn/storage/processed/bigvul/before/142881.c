void HTMLMediaElement::ResetMediaPlayerAndMediaSource() {
  CloseMediaSource();

  {
    AudioSourceProviderClientLockScope scope(*this);
    ClearMediaPlayerAndAudioSourceProviderClientWithoutLocking();
  }

  playing_remotely_ = false;
  RemoteRouteAvailabilityChanged(WebRemotePlaybackAvailability::kUnknown);

  if (audio_source_node_)
    GetAudioSourceProvider().SetClient(audio_source_node_);
}
