ScriptProcessorNode* ScriptProcessorNode::Create(
    BaseAudioContext& context,
    ExceptionState& exception_state) {
  DCHECK(IsMainThread());

  return Create(context, 0, 2, 2, exception_state);
}
