 void PresentationConnection::didFinishLoadingBlob(DOMArrayBuffer* buffer) {
   ASSERT(!m_messages.isEmpty() && m_messages.first()->type == MessageTypeBlob);
   ASSERT(buffer && buffer->buffer());
  WebPresentationClient* client = presentationClient(getExecutionContext());
  if (client) {
    client->sendBlobData(m_url, m_id,
                         static_cast<const uint8_t*>(buffer->data()),
                         buffer->byteLength(), m_proxy.get());
  }

  m_messages.removeFirst();
  m_blobLoader.clear();
  handleMessageQueue();
}
