void AudioNode::disconnect(AudioNode* destination,
                           unsigned output_index,
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

  unsigned number_of_disconnections = 0;
  for (unsigned input_index = 0; input_index < destination->numberOfInputs();
       ++input_index) {
    if (DisconnectFromOutputIfConnected(output_index, *destination,
                                        input_index))
      number_of_disconnections++;
  }

  if (number_of_disconnections == 0) {
    exception_state.ThrowDOMException(
        kInvalidAccessError,
        "output (" + String::Number(output_index) +
            ") is not connected to the given destination.");
  }
}
