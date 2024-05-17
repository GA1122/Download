ForwardingAudioStreamFactory* WebContentsImpl::GetAudioStreamFactory() {
  if (!audio_stream_factory_) {
    audio_stream_factory_.emplace(
        this,
        BrowserMainLoop::GetInstance()
            ? static_cast<media::UserInputMonitorBase*>(
                  BrowserMainLoop::GetInstance()->user_input_monitor())
            : nullptr,
        content::ServiceManagerConnection::GetForProcess()
            ->GetConnector()
            ->Clone(),
        AudioStreamBrokerFactory::CreateImpl());
  }

  return &*audio_stream_factory_;
}
