void HTMLMediaElement::AudioSourceProviderImpl::SetClient(
    AudioSourceProviderClient* client) {
  MutexLocker locker(provide_input_lock);

  if (client)
    client_ = MakeGarbageCollected<HTMLMediaElement::AudioClientImpl>(client);
  else
    client_.Clear();

  if (web_audio_source_provider_)
    web_audio_source_provider_->SetClient(client_.Get());
}
