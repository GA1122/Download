ScriptProcessorNode* ScriptProcessorNode::Create(
    BaseAudioContext& context,
    size_t buffer_size,
    ExceptionState& exception_state) {
  DCHECK(IsMainThread());

  return Create(context, buffer_size, 2, 2, exception_state);
}
