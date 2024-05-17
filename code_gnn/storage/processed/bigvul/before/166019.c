blink::WebRTCSessionDescription RTCPeerConnectionHandler::RemoteDescription() {
  DCHECK(task_runner_->RunsTasksInCurrentSequence());
  TRACE_EVENT0("webrtc", "RTCPeerConnectionHandler::remoteDescription");
  base::OnceCallback<const webrtc::SessionDescriptionInterface*()>
      description_cb =
          base::BindOnce(&webrtc::PeerConnectionInterface::remote_description,
                         native_peer_connection_);
  return GetWebRTCSessionDescriptionOnSignalingThread(std::move(description_cb),
                                                      "remoteDescription");
}
