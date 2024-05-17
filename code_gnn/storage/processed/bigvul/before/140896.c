PresentationConnection::PresentationConnection(LocalFrame* frame,
                                               const String& id,
                                               const KURL& url)
    : ContextClient(frame),
      m_id(id),
      m_url(url),
      m_state(WebPresentationConnectionState::Connecting),
      m_binaryType(BinaryTypeBlob),
      m_proxy(nullptr) {}
