void RenderFrameImpl::WillStartUsingPeerConnectionHandler(
    blink::WebRTCPeerConnectionHandler* handler) {
  static_cast<RTCPeerConnectionHandler*>(handler)->associateWithFrame(frame_);
}
