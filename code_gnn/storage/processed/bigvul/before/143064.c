DefaultAudioDestinationNode::DefaultAudioDestinationNode(
    BaseAudioContext& context,
    const WebAudioLatencyHint& latency_hint)
    : AudioDestinationNode(context) {
  SetHandler(DefaultAudioDestinationHandler::Create(*this, latency_hint));
}
