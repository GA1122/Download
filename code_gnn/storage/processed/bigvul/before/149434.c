Document* ContentSecurityPolicy::document() const {
  return (m_executionContext && m_executionContext->isDocument())
             ? toDocument(m_executionContext)
             : nullptr;
}
