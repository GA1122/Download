void throwPresentationDisconnectedError(ExceptionState& exceptionState) {
  exceptionState.throwDOMException(InvalidStateError,
                                   "Presentation connection is disconnected.");
}
