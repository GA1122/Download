void RTCPeerConnectionHandler::OnAddReceiverPlanB(
    RtpReceiverState receiver_state) {
  DCHECK(task_runner_->RunsTasksInCurrentSequence());
  DCHECK(receiver_state.is_initialized());
  TRACE_EVENT0("webrtc", "RTCPeerConnectionHandler::OnAddReceiverPlanB");
  auto web_track = receiver_state.track_ref()->web_track();
  track_metrics_.AddTrack(MediaStreamTrackMetrics::Direction::kReceive,
                          MediaStreamTrackMetricsKind(web_track),
                          web_track.Id().Utf8());
  for (const auto& stream_id : receiver_state.stream_ids()) {
    if (!IsRemoteStream(rtp_receivers_, stream_id))
      PerSessionWebRTCAPIMetrics::GetInstance()->IncrementStreamCounter();
  }
  uintptr_t receiver_id =
      RTCRtpReceiver::getId(receiver_state.webrtc_receiver().get());
  DCHECK(FindReceiver(receiver_id) == rtp_receivers_.end());
  auto rtp_receiver = std::make_unique<RTCRtpReceiver>(
      native_peer_connection_, std::move(receiver_state));
  rtp_receivers_.push_back(std::make_unique<RTCRtpReceiver>(*rtp_receiver));
  if (peer_connection_tracker_) {
    auto receiver_only_transceiver =
        std::make_unique<RTCRtpReceiverOnlyTransceiver>(
            std::make_unique<RTCRtpReceiver>(*rtp_receiver));
    size_t receiver_index = GetTransceiverIndex(*receiver_only_transceiver);
    peer_connection_tracker_->TrackAddTransceiver(
        this,
        PeerConnectionTracker::TransceiverUpdatedReason::kSetRemoteDescription,
        *receiver_only_transceiver.get(), receiver_index);
  }
  if (!is_closed_)
    client_->DidAddReceiverPlanB(rtp_receiver->ShallowCopy());
}
