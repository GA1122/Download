bool ContentSecurityPolicy::isActive() const {
  return !m_policies.isEmpty();
}
