DefaultAudioDestinationHandler::DefaultAudioDestinationHandler(
    AudioNode& node,
    const WebAudioLatencyHint& latency_hint)
    : AudioDestinationHandler(node),
      latency_hint_(latency_hint) {
  channel_count_ = 2;
  SetInternalChannelCountMode(kExplicit);
  SetInternalChannelInterpretation(AudioBus::kSpeakers);
}
