ChannelMergerNode* BaseAudioContext::createChannelMerger(
    uint32_t number_of_inputs,
    ExceptionState& exception_state) {
  DCHECK(IsMainThread());

  return ChannelMergerNode::Create(*this, number_of_inputs, exception_state);
}
