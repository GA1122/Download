ScriptProcessorNode* BaseAudioContext::createScriptProcessor(
    size_t buffer_size,
    ExceptionState& exception_state) {
  DCHECK(IsMainThread());

  return ScriptProcessorNode::Create(*this, buffer_size, exception_state);
}
