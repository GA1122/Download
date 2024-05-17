void RTCPeerConnectionHandler::RemoveTrackUnifiedPlanOnSignalingThread(
    rtc::scoped_refptr<webrtc::RtpSenderInterface> sender,
    TransceiverStateSurfacer* transceiver_state_surfacer,
    bool* result) {
  *result = native_peer_connection_->RemoveTrack(sender);
  std::vector<rtc::scoped_refptr<webrtc::RtpTransceiverInterface>> transceivers;
  if (*result) {
    rtc::scoped_refptr<webrtc::RtpTransceiverInterface> transceiver_for_sender =
        nullptr;
    for (const auto& transceiver : native_peer_connection_->GetTransceivers()) {
      if (transceiver->sender() == sender) {
        transceiver_for_sender = transceiver;
        break;
      }
    }
    DCHECK(transceiver_for_sender);
    transceivers = {transceiver_for_sender};
  }
  transceiver_state_surfacer->Initialize(track_adapter_map_,
                                         std::move(transceivers));
}
