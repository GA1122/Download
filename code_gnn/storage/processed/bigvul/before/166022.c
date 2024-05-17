RTCPeerConnectionHandler::RemoveTrackUnifiedPlan(
    blink::WebRTCRtpSender* web_sender) {
  DCHECK(task_runner_->RunsTasksInCurrentSequence());
  DCHECK_EQ(configuration_.sdp_semantics, webrtc::SdpSemantics::kUnifiedPlan);
  auto it = FindSender(web_sender->Id());
  if (it == rtp_senders_.end())
    return webrtc::RTCError(webrtc::RTCErrorType::INVALID_PARAMETER);
  const auto& sender = *it;
  auto webrtc_sender = sender->state().webrtc_sender();

  TransceiverStateSurfacer transceiver_state_surfacer(task_runner_,
                                                      signaling_thread());
  bool result;
  RunSynchronousClosureOnSignalingThread(
      base::BindRepeating(
          &RTCPeerConnectionHandler::RemoveTrackUnifiedPlanOnSignalingThread,
          base::Unretained(this), base::RetainedRef(webrtc_sender),
          base::Unretained(&transceiver_state_surfacer),
          base::Unretained(&result)),
      "RemoveTrackUnifiedPlanOnSignalingThread");
  DCHECK(transceiver_state_surfacer.is_initialized());
  if (!result) {
    transceiver_state_surfacer.ObtainStates();
    return webrtc::RTCError(webrtc::RTCErrorType::INTERNAL_ERROR);
  }

  auto transceiver_states = transceiver_state_surfacer.ObtainStates();
  DCHECK_EQ(transceiver_states.size(), 1u);
  auto transceiver_state = std::move(transceiver_states[0]);

  auto transceiver = CreateOrUpdateTransceiver(std::move(transceiver_state));
  if (peer_connection_tracker_) {
    size_t transceiver_index = GetTransceiverIndex(*transceiver);
    peer_connection_tracker_->TrackModifyTransceiver(
        this, PeerConnectionTracker::TransceiverUpdatedReason::kRemoveTrack,
        *transceiver.get(), transceiver_index);
  }
  std::unique_ptr<blink::WebRTCRtpTransceiver> web_transceiver =
      std::move(transceiver);
  return web_transceiver;
}
