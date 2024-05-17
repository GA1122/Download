bool PresentationConnection::canSendMessage(ExceptionState& exceptionState) {
  if (m_state != WebPresentationConnectionState::Connected) {
    throwPresentationDisconnectedError(exceptionState);
    return false;
  }

  return !!presentationClient(getExecutionContext());
}
