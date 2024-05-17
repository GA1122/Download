    clearMediaPlayerAndAudioSourceProviderClientWithoutLocking() {
  getAudioSourceProvider().setClient(nullptr);
  if (m_webMediaPlayer) {
    m_audioSourceProvider.wrap(nullptr);
    m_webMediaPlayer.reset();
  }
}
