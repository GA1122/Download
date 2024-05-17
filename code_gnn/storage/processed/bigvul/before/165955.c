void ConvertOfferOptionsToWebrtcOfferOptions(
    const blink::WebRTCOfferOptions& options,
    webrtc::PeerConnectionInterface::RTCOfferAnswerOptions* output) {
  output->offer_to_receive_audio = options.OfferToReceiveAudio();
  output->offer_to_receive_video = options.OfferToReceiveVideo();
  output->voice_activity_detection = options.VoiceActivityDetection();
  output->ice_restart = options.IceRestart();
}
