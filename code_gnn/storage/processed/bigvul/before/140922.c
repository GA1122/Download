void PresentationConnection::send(DOMArrayBuffer* arrayBuffer,
                                  ExceptionState& exceptionState) {
  ASSERT(arrayBuffer && arrayBuffer->buffer());
  if (!canSendMessage(exceptionState))
    return;

  m_messages.append(new Message(arrayBuffer));
  handleMessageQueue();
}
