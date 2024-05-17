void HTMLMediaElement::AudioSourceProviderImpl::setClient(
    AudioSourceProviderClient* client) {
  MutexLocker locker(provideInputLock);

  if (client)
    m_client = new HTMLMediaElement::AudioClientImpl(client);
  else
    m_client.clear();

  if (m_webAudioSourceProvider)
    m_webAudioSourceProvider->setClient(m_client.get());
}
