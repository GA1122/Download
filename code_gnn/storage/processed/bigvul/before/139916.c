  AudioSourceProviderClientLockScope(HTMLMediaElement& element)
      : m_client(element.audioSourceNode()) {
    if (m_client)
      m_client->lock();
  }
