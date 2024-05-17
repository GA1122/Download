  bool ReceiverWasRemoved(
      const RTCRtpReceiver& receiver,
      const std::vector<RtpTransceiverState>& transceiver_states) {
    for (const auto& transceiver_state : transceiver_states) {
      if (transceiver_state.receiver_state()->webrtc_receiver() ==
          receiver.state().webrtc_receiver()) {
        return false;
      }
    }
    return true;
  }
