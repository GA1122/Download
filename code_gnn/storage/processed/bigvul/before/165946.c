void RTCPeerConnectionHandler::AddTrackOnSignalingThread(
    rtc::scoped_refptr<webrtc::MediaStreamTrackInterface> track,
    std::vector<std::string> stream_ids,
    TransceiverStateSurfacer* transceiver_state_surfacer,
    webrtc::RTCErrorOr<rtc::scoped_refptr<webrtc::RtpSenderInterface>>*
        error_or_sender) {
  *error_or_sender = native_peer_connection_->AddTrack(track, stream_ids);
  std::vector<rtc::scoped_refptr<webrtc::RtpTransceiverInterface>> transceivers;
  if (error_or_sender->ok()) {
    auto sender = error_or_sender->value();
    if (configuration_.sdp_semantics == webrtc::SdpSemantics::kPlanB) {
      transceivers = {new SurfaceSenderStateOnly(sender)};
    } else {
      DCHECK_EQ(configuration_.sdp_semantics,
                webrtc::SdpSemantics::kUnifiedPlan);
      rtc::scoped_refptr<webrtc::RtpTransceiverInterface>
          transceiver_for_sender = nullptr;
      for (const auto& transceiver :
           native_peer_connection_->GetTransceivers()) {
        if (transceiver->sender() == sender) {
          transceiver_for_sender = transceiver;
          break;
        }
      }
      DCHECK(transceiver_for_sender);
      transceivers = {transceiver_for_sender};
    }
  }
  transceiver_state_surfacer->Initialize(track_adapter_map_,
                                         std::move(transceivers));
}
