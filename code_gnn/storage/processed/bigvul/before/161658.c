void AudioNode::disconnect(AudioNode* destination,
                           unsigned output_index,
                           unsigned input_index,
                           ExceptionState& exception_state) {
  DCHECK(IsMainThread());
  BaseAudioContext::GraphAutoLocker locker(context());

  if (output_index >= numberOfOutputs()) {
    exception_state.ThrowDOMException(
        kIndexSizeError,
        ExceptionMessages::IndexOutsideRange(
            "output index", output_index, 0u,
            ExceptionMessages::kInclusiveBound, numberOfOutputs() - 1,
            ExceptionMessages::kInclusiveBound));
    return;
  }

  if (input_index >= destination->Handler().NumberOfInputs()) {
    exception_state.ThrowDOMException(
        kIndexSizeError,
        ExceptionMessages::IndexOutsideRange(
            "input index", input_index, 0u, ExceptionMessages::kInclusiveBound,
            destination->numberOfInputs() - 1,
            ExceptionMessages::kInclusiveBound));
    return;
  }

  if (!DisconnectFromOutputIfConnected(output_index, *destination,
                                       input_index)) {
    exception_state.ThrowDOMException(
        kInvalidAccessError, "output (" + String::Number(output_index) +
                                 ") is not connected to the input (" +
                                 String::Number(input_index) +
                                 ") of the destination.");
    return;
  }
}
