void RenderFrameImpl::WillStartUsingPeerConnectionHandler(
    blink::WebRTCPeerConnectionHandler* handler) {
#if BUILDFLAG(ENABLE_WEBRTC)
  static_cast<RTCPeerConnectionHandler*>(handler)->associateWithFrame(frame_);
#endif
}
