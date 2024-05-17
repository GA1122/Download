size_t RTCPeerConnectionHandler::GetTransceiverIndex(
    const blink::WebRTCRtpTransceiver& web_transceiver) {
  if (web_transceiver.ImplementationType() ==
      blink::WebRTCRtpTransceiverImplementationType::kFullTransceiver) {
    for (size_t i = 0; i < rtp_transceivers_.size(); ++i) {
      if (web_transceiver.Id() == rtp_transceivers_[i]->Id())
        return i;
    }
  } else if (web_transceiver.ImplementationType() ==
             blink::WebRTCRtpTransceiverImplementationType::kPlanBSenderOnly) {
    const auto web_sender = web_transceiver.Sender();
    for (size_t i = 0; i < rtp_senders_.size(); ++i) {
      if (web_sender->Id() == rtp_senders_[i]->Id())
        return i;
    }
  } else {
    RTC_DCHECK(
        web_transceiver.ImplementationType() ==
        blink::WebRTCRtpTransceiverImplementationType::kPlanBReceiverOnly);
    const auto web_receiver = web_transceiver.Receiver();
    for (size_t i = 0; i < rtp_receivers_.size(); ++i) {
      if (web_receiver->Id() == rtp_receivers_[i]->Id())
        return i;
    }
  }
  NOTREACHED();
  return 0u;
}
