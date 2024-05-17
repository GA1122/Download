RTCPeerConnectionHandler::GetWebRTCSessionDescriptionOnSignalingThread(
    base::OnceCallback<const webrtc::SessionDescriptionInterface*()>
        description_cb,
    const char* log_text) {

  blink::WebRTCSessionDescription description;   
  RunSynchronousOnceClosureOnSignalingThread(
      base::BindOnce(&GetWebRTCSessionDescriptionFromSessionDescriptionCallback,
                     std::move(description_cb), base::Unretained(&description)),
      log_text);
  return description;
}
