RTCPeerConnectionHandler::AddTransceiverWithTrack(
    const blink::WebMediaStreamTrack& web_track,
    const webrtc::RtpTransceiverInit& init) {
  DCHECK_EQ(configuration_.sdp_semantics, webrtc::SdpSemantics::kUnifiedPlan);
  DCHECK(task_runner_->RunsTasksInCurrentSequence());
  std::unique_ptr<WebRtcMediaStreamTrackAdapterMap::AdapterRef> track_ref =
      track_adapter_map_->GetOrCreateLocalTrackAdapter(web_track);
  TransceiverStateSurfacer transceiver_state_surfacer(task_runner_,
                                                      signaling_thread());
  webrtc::RTCErrorOr<rtc::scoped_refptr<webrtc::RtpTransceiverInterface>>
      error_or_transceiver;
  RunSynchronousClosureOnSignalingThread(
      base::BindRepeating(
          &RTCPeerConnectionHandler::AddTransceiverWithTrackOnSignalingThread,
          base::Unretained(this), base::RetainedRef(track_ref->webrtc_track()),
          base::ConstRef(init), base::Unretained(&transceiver_state_surfacer),
          base::Unretained(&error_or_transceiver)),
      "AddTransceiverWithTrackOnSignalingThread");
  if (!error_or_transceiver.ok()) {
    transceiver_state_surfacer.ObtainStates();
    return error_or_transceiver.MoveError();
  }

  auto transceiver_states = transceiver_state_surfacer.ObtainStates();
  auto transceiver =
      CreateOrUpdateTransceiver(std::move(transceiver_states[0]));
  std::unique_ptr<blink::WebRTCRtpTransceiver> web_transceiver =
      std::move(transceiver);
  return std::move(web_transceiver);
}
