void PresentationConnection::send(DOMArrayBufferView* arrayBufferView,
                                  ExceptionState& exceptionState) {
  ASSERT(arrayBufferView);
  if (!canSendMessage(exceptionState))
    return;

  m_messages.append(new Message(arrayBufferView->buffer()));
  handleMessageQueue();
}
