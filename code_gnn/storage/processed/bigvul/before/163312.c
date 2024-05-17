RenderThreadImpl::CreateMediaStreamCenter(
    blink::WebMediaStreamCenterClient* client) {
  std::unique_ptr<blink::WebMediaStreamCenter> media_stream_center;
#if BUILDFLAG(ENABLE_WEBRTC)
  if (!media_stream_center) {
    media_stream_center =
        GetContentClient()->renderer()->OverrideCreateWebMediaStreamCenter(
            client);
    if (!media_stream_center) {
      media_stream_center = std::make_unique<MediaStreamCenter>(
          client, GetPeerConnectionDependencyFactory());
    }
  }
#endif
  return media_stream_center;
}
