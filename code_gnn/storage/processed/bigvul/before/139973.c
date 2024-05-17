void HTMLMediaElement::dispose() {
  closeMediaSource();

  clearMediaPlayerAndAudioSourceProviderClientWithoutLocking();
}
