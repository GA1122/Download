RTCPeerConnectionHandler::AddTransceiverWithKind(
    std::string kind,
    const webrtc::RtpTransceiverInit& init) {
  DCHECK_EQ(configuration_.sdp_semantics, webrtc::SdpSemantics::kUnifiedPlan);
  DCHECK(task_runner_->RunsTasksInCurrentSequence());
  cricket::MediaType media_type;
  if (kind == webrtc::MediaStreamTrackInterface::kAudioKind) {
    media_type = cricket::MEDIA_TYPE_AUDIO;
  } else {
    DCHECK_EQ(kind, webrtc::MediaStreamTrackInterface::kVideoKind);
    media_type = cricket::MEDIA_TYPE_VIDEO;
  }
  TransceiverStateSurfacer transceiver_state_surfacer(task_runner_,
                                                      signaling_thread());
  webrtc::RTCErrorOr<rtc::scoped_refptr<webrtc::RtpTransceiverInterface>>
      error_or_transceiver;
  RunSynchronousClosureOnSignalingThread(
      base::BindRepeating(&RTCPeerConnectionHandler::
                              AddTransceiverWithMediaTypeOnSignalingThread,
                          base::Unretained(this), base::ConstRef(media_type),
                          base::ConstRef(init),
                          base::Unretained(&transceiver_state_surfacer),
                          base::Unretained(&error_or_transceiver)),
      "AddTransceiverWithMediaTypeOnSignalingThread");
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
