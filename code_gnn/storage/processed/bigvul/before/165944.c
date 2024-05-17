bool RTCPeerConnectionHandler::AddICECandidate(
    scoped_refptr<blink::WebRTCICECandidate> candidate) {
  DCHECK(task_runner_->RunsTasksInCurrentSequence());
  TRACE_EVENT0("webrtc", "RTCPeerConnectionHandler::addICECandidate");
  std::unique_ptr<webrtc::IceCandidateInterface> native_candidate(
      dependency_factory_->CreateIceCandidate(candidate->SdpMid().Utf8(),
                                              candidate->SdpMLineIndex(),
                                              candidate->Candidate().Utf8()));
  bool return_value = false;

  if (native_candidate) {
    return_value =
        native_peer_connection_->AddIceCandidate(native_candidate.get());
    LOG_IF(ERROR, !return_value) << "Error processing ICE candidate.";
  } else {
    LOG(ERROR) << "Could not create native ICE candidate.";
  }

  if (peer_connection_tracker_) {
    peer_connection_tracker_->TrackAddIceCandidate(
        this, std::move(candidate), PeerConnectionTracker::SOURCE_REMOTE,
        return_value);
  }
  return return_value;
}
