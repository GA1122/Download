void PresentationConnection::close() {
  if (m_state != WebPresentationConnectionState::Connecting &&
      m_state != WebPresentationConnectionState::Connected) {
    return;
  }
  WebPresentationClient* client = presentationClient(getExecutionContext());
  if (client)
    client->closeSession(m_url, m_id, m_proxy.get());

  tearDown();
}
