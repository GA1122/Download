void AudioNode::connect(AudioParam* param,
                        unsigned output_index,
                        ExceptionState& exception_state) {
  DCHECK(IsMainThread());
  BaseAudioContext::GraphAutoLocker locker(context());

  if (context()->IsContextClosed()) {
    exception_state.ThrowDOMException(
        kInvalidStateError,
        "Cannot connect after the context has been closed.");
    return;
  }

  if (!param) {
    exception_state.ThrowDOMException(kSyntaxError, "invalid AudioParam.");
    return;
  }

  if (output_index >= numberOfOutputs()) {
    exception_state.ThrowDOMException(
        kIndexSizeError, "output index (" + String::Number(output_index) +
                             ") exceeds number of outputs (" +
                             String::Number(numberOfOutputs()) + ").");
    return;
  }

  if (context() != param->Context()) {
    exception_state.ThrowDOMException(
        kSyntaxError,
        "cannot connect to an AudioParam "
        "belonging to a different audio context.");
    return;
  }

  param->Handler().Connect(Handler().Output(output_index));
  if (!connected_params_[output_index])
    connected_params_[output_index] = new HeapHashSet<Member<AudioParam>>();
  connected_params_[output_index]->insert(param);
}
