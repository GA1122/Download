WebKit::WebMediaStreamCenter* RenderThreadImpl::CreateMediaStreamCenter(
    WebKit::WebMediaStreamCenterClient* client) {
#if defined(ENABLE_WEBRTC)
  if (!CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kEnableMediaStream)) {
    return NULL;
  }
  if (!media_stream_center_)
    media_stream_center_ = new content::MediaStreamCenter(client);
#endif
  return media_stream_center_;
}
