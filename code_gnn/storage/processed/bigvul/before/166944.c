MediaStreamAudioDestinationNode* BaseAudioContext::createMediaStreamDestination(
    ExceptionState& exception_state) {
  DCHECK(IsMainThread());

  return MediaStreamAudioDestinationNode::Create(*this, 2, exception_state);
}
