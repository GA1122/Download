ScriptProcessorNode* ScriptProcessorNode::Create(
    BaseAudioContext& context,
    size_t buffer_size,
    unsigned number_of_input_channels,
    ExceptionState& exception_state) {
  DCHECK(IsMainThread());

  return Create(context, buffer_size, number_of_input_channels, 2,
                exception_state);
}
