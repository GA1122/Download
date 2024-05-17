bool ContentSecurityPolicy::allowFontFromSource(
    const KURL& url,
    RedirectStatus redirectStatus,
    SecurityViolationReportingPolicy reportingPolicy) const {
  return isAllowedByAll<&CSPDirectiveList::allowFontFromSource>(
      m_policies, url, redirectStatus, reportingPolicy);
}
