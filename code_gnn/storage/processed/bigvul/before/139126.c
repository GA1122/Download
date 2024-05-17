void RenderProcessHostImpl::ClearWebRtcLogMessageCallback() {
#if BUILDFLAG(ENABLE_WEBRTC)
  BrowserMainLoop::GetInstance()
      ->media_stream_manager()
      ->UnregisterNativeLogCallback(GetID());
#endif
}
