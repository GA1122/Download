RTCPeerConnectionHandler::CurrentRemoteDescription() {
  DCHECK(task_runner_->RunsTasksInCurrentSequence());
  TRACE_EVENT0("webrtc", "RTCPeerConnectionHandler::currentRemoteDescription");
  base::OnceCallback<const webrtc::SessionDescriptionInterface*()>
      description_cb = base::BindOnce(
          &webrtc::PeerConnectionInterface::current_remote_description,
          native_peer_connection_);
  return GetWebRTCSessionDescriptionOnSignalingThread(
      std::move(description_cb), "currentRemoteDescription");
}
