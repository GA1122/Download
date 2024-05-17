void PresentationConnection::send(const String& message,
                                  ExceptionState& exceptionState) {
  if (!canSendMessage(exceptionState))
    return;

  m_messages.append(new Message(message));
  handleMessageQueue();
}
