const KURL ContentSecurityPolicy::url() const {
  return m_executionContext->contextURL();
}
