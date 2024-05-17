bool ContentSecurityPolicy::allowBaseURI(
    const KURL& url,
    RedirectStatus redirectStatus,
    SecurityViolationReportingPolicy reportingPolicy) const {
  return isAllowedByAll<&CSPDirectiveList::allowBaseURI>(
      m_policies, url, redirectStatus, reportingPolicy);
}
