void RTCPeerConnectionHandler::OnRemoveReceiverPlanB(uintptr_t receiver_id) {
  DCHECK(task_runner_->RunsTasksInCurrentSequence());
  TRACE_EVENT0("webrtc", "RTCPeerConnectionHandler::OnRemoveReceiverPlanB");

  auto it = FindReceiver(receiver_id);
  DCHECK(it != rtp_receivers_.end());
  auto receiver = std::make_unique<RTCRtpReceiver>(*(*it));
  track_metrics_.RemoveTrack(MediaStreamTrackMetrics::Direction::kReceive,
                             MediaStreamTrackMetricsKind(receiver->Track()),
                             receiver->Track().Id().Utf8());
  if (peer_connection_tracker_) {
    auto receiver_only_transceiver =
        std::make_unique<RTCRtpReceiverOnlyTransceiver>(
            std::make_unique<RTCRtpReceiver>(*receiver));
    size_t receiver_index = GetTransceiverIndex(*receiver_only_transceiver);
    peer_connection_tracker_->TrackRemoveTransceiver(
        this,
        PeerConnectionTracker::TransceiverUpdatedReason::kSetRemoteDescription,
        *receiver_only_transceiver.get(), receiver_index);
  }
  rtp_receivers_.erase(it);
  for (const auto& stream_id : receiver->state().stream_ids()) {
    if (!IsRemoteStream(rtp_receivers_, stream_id))
      PerSessionWebRTCAPIMetrics::GetInstance()->IncrementStreamCounter();
  }
  if (!is_closed_)
    client_->DidRemoveReceiverPlanB(std::move(receiver));
}
