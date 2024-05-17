RTCPeerConnectionHandler::PendingRemoteDescription() {
  DCHECK(task_runner_->RunsTasksInCurrentSequence());
  TRACE_EVENT0("webrtc", "RTCPeerConnectionHandler::pendingRemoteDescription");
  base::OnceCallback<const webrtc::SessionDescriptionInterface*()>
      description_cb = base::BindOnce(
          &webrtc::PeerConnectionInterface::pending_remote_description,
          native_peer_connection_);
  return GetWebRTCSessionDescriptionOnSignalingThread(
      std::move(description_cb), "pendingRemoteDescription");
}
