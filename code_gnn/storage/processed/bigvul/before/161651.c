AudioNode* AudioNode::connect(AudioNode* destination,
                              unsigned output_index,
                              unsigned input_index,
                              ExceptionState& exception_state) {
  DCHECK(IsMainThread());
  BaseAudioContext::GraphAutoLocker locker(context());

  if (context()->IsContextClosed()) {
    exception_state.ThrowDOMException(
        kInvalidStateError,
        "Cannot connect after the context has been closed.");
    return nullptr;
  }

  if (!destination) {
    exception_state.ThrowDOMException(kSyntaxError,
                                      "invalid destination node.");
    return nullptr;
  }

  if (output_index >= numberOfOutputs()) {
    exception_state.ThrowDOMException(
        kIndexSizeError, "output index (" + String::Number(output_index) +
                             ") exceeds number of outputs (" +
                             String::Number(numberOfOutputs()) + ").");
    return nullptr;
  }

  if (destination && input_index >= destination->numberOfInputs()) {
    exception_state.ThrowDOMException(
        kIndexSizeError, "input index (" + String::Number(input_index) +
                             ") exceeds number of inputs (" +
                             String::Number(destination->numberOfInputs()) +
                             ").");
    return nullptr;
  }

  if (context() != destination->context()) {
    exception_state.ThrowDOMException(
        kInvalidAccessError,
        "cannot connect to a destination "
        "belonging to a different audio context.");
    return nullptr;
  }

  if (Handler().GetNodeType() == AudioHandler::kNodeTypeScriptProcessor &&
      Handler().NumberOfOutputChannels() == 0) {
    exception_state.ThrowDOMException(kInvalidAccessError,
                                      "cannot connect a ScriptProcessorNode "
                                      "with 0 output channels to any "
                                      "destination node.");
    return nullptr;
  }

  destination->Handler()
      .Input(input_index)
      .Connect(Handler().Output(output_index));
  if (!connected_nodes_[output_index])
    connected_nodes_[output_index] = new HeapHashSet<Member<AudioNode>>();
  connected_nodes_[output_index]->insert(destination);

  context()->IncrementConnectionCount();

  return destination;
}
