RenderProcessHostImpl::GetRendererAudioOutputStreamFactoryContext() {
  if (!audio_output_stream_factory_context_) {
    media::AudioManager* audio_manager =
        BrowserMainLoop::GetInstance()->audio_manager();
    MediaStreamManager* media_stream_manager =
        BrowserMainLoop::GetInstance()->media_stream_manager();
    media::AudioSystem* audio_system =
        BrowserMainLoop::GetInstance()->audio_system();
    audio_output_stream_factory_context_.reset(
        new RendererAudioOutputStreamFactoryContextImpl(
            GetID(), audio_system, audio_manager, media_stream_manager));
  }
  return audio_output_stream_factory_context_.get();
}
