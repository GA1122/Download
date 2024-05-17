void LocalFrameClientImpl::DispatchWillStartUsingPeerConnectionHandler(
    WebRTCPeerConnectionHandler* handler) {
  web_frame_->Client()->WillStartUsingPeerConnectionHandler(handler);
}
