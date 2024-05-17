void PresentationConnection::tearDown() {
  if (m_blobLoader) {
    m_blobLoader->cancel();
    m_blobLoader.clear();
  }
  m_messages.clear();
}
