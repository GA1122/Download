RenderFrameImpl::GetAudioInputStreamFactory() {
  if (!audio_input_stream_factory_)
    GetRemoteInterfaces()->GetInterface(&audio_input_stream_factory_);
  return audio_input_stream_factory_.get();
}
