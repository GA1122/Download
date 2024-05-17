void HTMLMediaElement::Dispose() {
  CloseMediaSource();

  ClearMediaPlayerAndAudioSourceProviderClientWithoutLocking();
}
