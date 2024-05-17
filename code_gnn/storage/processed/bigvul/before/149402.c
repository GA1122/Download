bool ContentSecurityPolicy::allowConnectToSource(
    const KURL& url,
    RedirectStatus redirectStatus,
    SecurityViolationReportingPolicy reportingPolicy) const {
  return isAllowedByAll<&CSPDirectiveList::allowConnectToSource>(
      m_policies, url, redirectStatus, reportingPolicy);
}
