void BaseAudioContext::ThrowExceptionForClosedState(
    ExceptionState& exception_state) {
  exception_state.ThrowDOMException(kInvalidStateError,
                                    "AudioContext has been closed.");
}
