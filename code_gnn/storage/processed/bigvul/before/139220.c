void RenderProcessHostImpl::SetWebRtcLogMessageCallback(
    base::Callback<void(const std::string&)> callback) {
#if BUILDFLAG(ENABLE_WEBRTC)
  BrowserMainLoop::GetInstance()->media_stream_manager()->
      RegisterNativeLogCallback(GetID(), callback);
#endif
}
