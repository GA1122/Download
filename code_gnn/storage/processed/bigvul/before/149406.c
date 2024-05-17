bool ContentSecurityPolicy::allowFrameFromSource(
    const KURL& url,
    RedirectStatus redirectStatus,
    SecurityViolationReportingPolicy reportingPolicy) const {
  return isAllowedByAll<&CSPDirectiveList::allowFrameFromSource>(
      m_policies, url, redirectStatus, reportingPolicy);
}
