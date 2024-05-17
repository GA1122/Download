void ContentSecurityPolicy::addAndReportPolicyFromHeaderValue(
    const String& header,
    ContentSecurityPolicyHeaderType type,
    ContentSecurityPolicyHeaderSource source) {
  size_t previousPolicyCount = m_policies.size();
  addPolicyFromHeaderValue(header, type, source);
  if (document() && document()->frame()) {

    std::vector<blink::WebContentSecurityPolicyPolicy> policies;
    for (size_t i = previousPolicyCount; i < m_policies.size(); ++i)
      policies.push_back(m_policies[i]->exposeForNavigationalChecks());
    document()->frame()->client()->didAddContentSecurityPolicy(
        header, type, source, policies);
  }
}
