void PresentationConnection::handleMessageQueue() {
  WebPresentationClient* client = presentationClient(getExecutionContext());
  if (!client || !m_proxy)
    return;

  while (!m_messages.isEmpty() && !m_blobLoader) {
    Message* message = m_messages.first().get();
    switch (message->type) {
      case MessageTypeText:
        client->sendString(m_url, m_id, message->text, m_proxy.get());
        m_messages.removeFirst();
        break;
      case MessageTypeArrayBuffer:
        client->sendArrayBuffer(
            m_url, m_id,
            static_cast<const uint8_t*>(message->arrayBuffer->data()),
            message->arrayBuffer->byteLength(), m_proxy.get());
        m_messages.removeFirst();
        break;
      case MessageTypeBlob:
        ASSERT(!m_blobLoader);
        m_blobLoader = new BlobLoader(message->blobDataHandle, this);
        break;
    }
  }
}
