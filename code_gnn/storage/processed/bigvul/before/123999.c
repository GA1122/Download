void RenderViewImpl::willStartUsingPeerConnectionHandler(
    WebKit::WebFrame* frame, WebKit::WebRTCPeerConnectionHandler* handler) {
#if defined(ENABLE_WEBRTC)
  static_cast<RTCPeerConnectionHandler*>(handler)->associateWithFrame(frame);
#endif
}
