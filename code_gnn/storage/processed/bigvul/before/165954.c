void ConvertConstraintsToWebrtcOfferOptions(
    const blink::WebMediaConstraints& constraints,
    webrtc::PeerConnectionInterface::RTCOfferAnswerOptions* output) {
  if (constraints.IsEmpty()) {
    return;
  }
  std::string failing_name;
  if (constraints.Basic().HasMandatoryOutsideSet(
          {constraints.Basic().offer_to_receive_audio.GetName(),
           constraints.Basic().offer_to_receive_video.GetName(),
           constraints.Basic().voice_activity_detection.GetName(),
           constraints.Basic().ice_restart.GetName()},
          failing_name)) {
    DLOG(ERROR) << "Invalid mandatory constraint to CreateOffer/Answer: "
                << failing_name;
  }
  GetConstraintValueAsInteger(
      constraints, &blink::WebMediaTrackConstraintSet::offer_to_receive_audio,
      &output->offer_to_receive_audio);
  GetConstraintValueAsInteger(
      constraints, &blink::WebMediaTrackConstraintSet::offer_to_receive_video,
      &output->offer_to_receive_video);
  GetConstraintValueAsBoolean(
      constraints, &blink::WebMediaTrackConstraintSet::voice_activity_detection,
      &output->voice_activity_detection);
  GetConstraintValueAsBoolean(constraints,
                              &blink::WebMediaTrackConstraintSet::ice_restart,
                              &output->ice_restart);
}
