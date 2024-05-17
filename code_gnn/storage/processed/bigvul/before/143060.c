DefaultAudioDestinationHandler::Create(
    AudioNode& node,
    const WebAudioLatencyHint& latency_hint) {
  return base::AdoptRef(new DefaultAudioDestinationHandler(node, latency_hint));
}
