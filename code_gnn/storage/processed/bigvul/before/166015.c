RTCPeerConnectionHandler::PendingLocalDescription() {
  DCHECK(task_runner_->RunsTasksInCurrentSequence());
  TRACE_EVENT0("webrtc", "RTCPeerConnectionHandler::pendingLocalDescription");
  base::OnceCallback<const webrtc::SessionDescriptionInterface*()>
      description_cb = base::BindOnce(
          &webrtc::PeerConnectionInterface::pending_local_description,
          native_peer_connection_);
  return GetWebRTCSessionDescriptionOnSignalingThread(
      std::move(description_cb), "pendingLocalDescription");
}
