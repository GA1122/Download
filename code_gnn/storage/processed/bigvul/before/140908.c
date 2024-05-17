void PresentationConnection::didFailLoadingBlob(
    FileError::ErrorCode errorCode) {
  ASSERT(!m_messages.isEmpty() && m_messages.first()->type == MessageTypeBlob);
  m_messages.removeFirst();
  m_blobLoader.clear();
  handleMessageQueue();
}
