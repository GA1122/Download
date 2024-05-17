ScriptProcessorNode* BaseAudioContext::createScriptProcessor(
    size_t buffer_size,
    size_t number_of_input_channels,
    size_t number_of_output_channels,
    ExceptionState& exception_state) {
  DCHECK(IsMainThread());

  return ScriptProcessorNode::Create(
      *this, buffer_size, number_of_input_channels, number_of_output_channels,
      exception_state);
}
