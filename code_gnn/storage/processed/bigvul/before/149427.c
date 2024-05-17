KURL ContentSecurityPolicy::completeURL(const String& url) const {
  return m_executionContext->contextCompleteURL(url);
}
