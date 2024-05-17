void AudioNode::setChannelInterpretation(const String& interpretation,
                                         ExceptionState& exception_state) {
  Handler().SetChannelInterpretation(interpretation, exception_state);
}
