bool ContentSecurityPolicy::allowRequestWithoutIntegrity(
    WebURLRequest::RequestContext context,
    const KURL& url,
    RedirectStatus redirectStatus,
    SecurityViolationReportingPolicy reportingPolicy) const {
  for (const auto& policy : m_policies) {
    if (!policy->allowRequestWithoutIntegrity(context, url, redirectStatus,
                                              reportingPolicy))
      return false;
  }
  return true;
}
