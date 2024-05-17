DEFINE_TRACE(PresentationConnection) {
  visitor->trace(m_blobLoader);
  visitor->trace(m_messages);
  EventTargetWithInlineData::trace(visitor);
  ContextClient::trace(visitor);
}
