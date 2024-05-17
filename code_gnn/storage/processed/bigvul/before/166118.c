void RenderFrameHostImpl::CreateAudioOutputStreamFactory(
    mojom::RendererAudioOutputStreamFactoryRequest request) {
  if (base::FeatureList::IsEnabled(features::kAudioServiceAudioStreams)) {
    media::AudioSystem* audio_system =
        BrowserMainLoop::GetInstance()->audio_system();
    MediaStreamManager* media_stream_manager =
        BrowserMainLoop::GetInstance()->media_stream_manager();
    audio_service_audio_output_stream_factory_.emplace(
        this, audio_system, media_stream_manager, std::move(request));
  } else {
    RendererAudioOutputStreamFactoryContext* factory_context =
        GetProcess()->GetRendererAudioOutputStreamFactoryContext();
    DCHECK(factory_context);
    in_content_audio_output_stream_factory_ =
        RenderFrameAudioOutputStreamFactoryHandle::CreateFactory(
            factory_context, GetRoutingID(), std::move(request));
   }
 }
