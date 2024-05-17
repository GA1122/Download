  ~AudioSourceProviderClientLockScope() {
    if (m_client)
      m_client->unlock();
  }
