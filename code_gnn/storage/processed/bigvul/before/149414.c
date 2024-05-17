bool ContentSecurityPolicy::allowObjectFromSource(
    const KURL& url,
    RedirectStatus redirectStatus,
    SecurityViolationReportingPolicy reportingPolicy) const {
  return isAllowedByAll<&CSPDirectiveList::allowObjectFromSource>(
      m_policies, url, redirectStatus, reportingPolicy);
}
