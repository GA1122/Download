void HTMLMediaElement::AudioSourceProviderImpl::provideInput(
    AudioBus* bus,
    size_t framesToProcess) {
  DCHECK(bus);

  MutexTryLocker tryLocker(provideInputLock);
  if (!tryLocker.locked() || !m_webAudioSourceProvider || !m_client.get()) {
    bus->zero();
    return;
  }

  size_t n = bus->numberOfChannels();
  WebVector<float*> webAudioData(n);
  for (size_t i = 0; i < n; ++i)
    webAudioData[i] = bus->channel(i)->mutableData();

  m_webAudioSourceProvider->provideInput(webAudioData, framesToProcess);
}
