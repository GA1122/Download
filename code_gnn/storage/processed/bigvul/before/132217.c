void RenderFrameImpl::InitializeUserMediaClient() {
  if (!RenderThreadImpl::current())   
    return;

#if defined(OS_ANDROID)
  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kDisableWebRTC))
    return;
#endif

#if defined(ENABLE_WEBRTC)
  DCHECK(!web_user_media_client_);
  web_user_media_client_ = new UserMediaClientImpl(
      this,
      RenderThreadImpl::current()->GetPeerConnectionDependencyFactory(),
      make_scoped_ptr(new MediaStreamDispatcher(this)).Pass());
#endif
}
