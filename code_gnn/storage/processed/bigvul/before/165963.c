RTCPeerConnectionHandler::CreateOrUpdateTransceiver(
    RtpTransceiverState transceiver_state) {
  DCHECK_EQ(configuration_.sdp_semantics, webrtc::SdpSemantics::kUnifiedPlan);
  DCHECK(transceiver_state.is_initialized());
  DCHECK(transceiver_state.sender_state());
  DCHECK(transceiver_state.receiver_state());
  auto webrtc_transceiver = transceiver_state.webrtc_transceiver();
  auto webrtc_sender = transceiver_state.sender_state()->webrtc_sender();
  auto webrtc_receiver = transceiver_state.receiver_state()->webrtc_receiver();

  std::unique_ptr<RTCRtpTransceiver> transceiver;
  auto it = FindTransceiver(RTCRtpTransceiver::GetId(webrtc_transceiver.get()));
  if (it == rtp_transceivers_.end()) {
    transceiver = std::make_unique<RTCRtpTransceiver>(
        native_peer_connection_, track_adapter_map_,
        std::move(transceiver_state));
    rtp_transceivers_.push_back(transceiver->ShallowCopy());
    DCHECK(FindSender(RTCRtpSender::getId(webrtc_sender.get())) ==
           rtp_senders_.end());
    rtp_senders_.push_back(
        std::make_unique<RTCRtpSender>(*transceiver->content_sender()));
    DCHECK(FindReceiver(RTCRtpReceiver::getId(webrtc_receiver.get())) ==
           rtp_receivers_.end());
    rtp_receivers_.push_back(
        std::make_unique<RTCRtpReceiver>(*transceiver->content_receiver()));
  } else {
    transceiver = (*it)->ShallowCopy();
    transceiver->set_state(std::move(transceiver_state));
    DCHECK(FindSender(RTCRtpSender::getId(webrtc_sender.get())) !=
           rtp_senders_.end());
    DCHECK(FindReceiver(RTCRtpReceiver::getId(webrtc_receiver.get())) !=
           rtp_receivers_.end());
  }
  return transceiver;
}
