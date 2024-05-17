void RenderFrameHostImpl::CreateAudioOutputStreamFactory(
    mojom::RendererAudioOutputStreamFactoryRequest request) {
  RendererAudioOutputStreamFactoryContext* factory_context =
      GetProcess()->GetRendererAudioOutputStreamFactoryContext();
  DCHECK(factory_context);
  audio_output_stream_factory_ =
      RenderFrameAudioOutputStreamFactoryHandle::CreateFactory(
          factory_context, GetRoutingID(), std::move(request));
}
