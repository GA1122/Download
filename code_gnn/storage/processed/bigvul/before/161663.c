void AudioNode::setChannelCount(unsigned long count,
                                ExceptionState& exception_state) {
  Handler().SetChannelCount(count, exception_state);
}
