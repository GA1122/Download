ForwardingAudioStreamFactory* WebContentsImpl::GetAudioStreamFactory() {
  if (!audio_stream_factory_) {
    audio_stream_factory_.emplace(
        this,
        content::ServiceManagerConnection::GetForProcess()
            ->GetConnector()
            ->Clone(),
        AudioStreamBrokerFactory::CreateImpl());
  }

  return &*audio_stream_factory_;
}
