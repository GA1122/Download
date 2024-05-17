blink::WebRTCSessionDescription RTCPeerConnectionHandler::LocalDescription() {
  DCHECK(task_runner_->RunsTasksInCurrentSequence());
  TRACE_EVENT0("webrtc", "RTCPeerConnectionHandler::localDescription");

  base::OnceCallback<const webrtc::SessionDescriptionInterface*()>
      description_cb =
          base::BindOnce(&webrtc::PeerConnectionInterface::local_description,
                         native_peer_connection_);
  return GetWebRTCSessionDescriptionOnSignalingThread(std::move(description_cb),
                                                      "localDescription");
}
