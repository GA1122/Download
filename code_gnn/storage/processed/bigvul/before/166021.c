bool RTCPeerConnectionHandler::RemoveTrackPlanB(
    blink::WebRTCRtpSender* web_sender) {
  DCHECK_EQ(configuration_.sdp_semantics, webrtc::SdpSemantics::kPlanB);
  auto web_track = web_sender->Track();
  auto it = FindSender(web_sender->Id());
  if (it == rtp_senders_.end())
    return false;
  if (!(*it)->RemoveFromPeerConnection(native_peer_connection_.get()))
    return false;
  track_metrics_.RemoveTrack(MediaStreamTrackMetrics::Direction::kSend,
                             MediaStreamTrackMetricsKind(web_track),
                             web_track.Id().Utf8());
  if (peer_connection_tracker_) {
    auto sender_only_transceiver =
        std::make_unique<RTCRtpSenderOnlyTransceiver>(
            std::make_unique<RTCRtpSender>(*it->get()));
    size_t sender_index = GetTransceiverIndex(*sender_only_transceiver);
    peer_connection_tracker_->TrackRemoveTransceiver(
        this, PeerConnectionTracker::TransceiverUpdatedReason::kRemoveTrack,
        *sender_only_transceiver.get(), sender_index);
  }
  std::vector<std::string> stream_ids = (*it)->state().stream_ids();
  rtp_senders_.erase(it);
  for (const auto& stream_id : stream_ids) {
    if (GetLocalStreamUsageCount(rtp_senders_, stream_id) == 0u) {
      PerSessionWebRTCAPIMetrics::GetInstance()->DecrementStreamCounter();
    }
  }
  return true;
}
