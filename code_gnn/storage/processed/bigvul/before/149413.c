bool ContentSecurityPolicy::allowMediaFromSource(
    const KURL& url,
    RedirectStatus redirectStatus,
    SecurityViolationReportingPolicy reportingPolicy) const {
  return isAllowedByAll<&CSPDirectiveList::allowMediaFromSource>(
      m_policies, url, redirectStatus, reportingPolicy);
}
