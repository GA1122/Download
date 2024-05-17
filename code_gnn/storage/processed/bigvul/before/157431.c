ChannelSplitterNode* BaseAudioContext::createChannelSplitter(
    size_t number_of_outputs,
    ExceptionState& exception_state) {
  DCHECK(IsMainThread());

  return ChannelSplitterNode::Create(*this, number_of_outputs, exception_state);
}
