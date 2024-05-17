RTCPeerConnectionHandler::RemoveTrack(blink::WebRTCRtpSender* web_sender) {
  DCHECK(task_runner_->RunsTasksInCurrentSequence());
  TRACE_EVENT0("webrtc", "RTCPeerConnectionHandler::RemoveTrack");
  if (configuration_.sdp_semantics == webrtc::SdpSemantics::kPlanB) {
    if (RemoveTrackPlanB(web_sender)) {
      std::unique_ptr<blink::WebRTCRtpTransceiver> web_transceiver = nullptr;
      return std::move(web_transceiver);
    }
    return webrtc::RTCError(webrtc::RTCErrorType::INVALID_STATE);
  }
  DCHECK_EQ(configuration_.sdp_semantics, webrtc::SdpSemantics::kUnifiedPlan);
  return RemoveTrackUnifiedPlan(web_sender);
}
