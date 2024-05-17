bool ContentSecurityPolicy::urlMatchesSelf(const KURL& url) const {
  return m_selfSource->matches(url, RedirectStatus::NoRedirect);
}
