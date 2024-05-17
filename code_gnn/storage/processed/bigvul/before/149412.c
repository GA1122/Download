bool ContentSecurityPolicy::allowManifestFromSource(
    const KURL& url,
    RedirectStatus redirectStatus,
    SecurityViolationReportingPolicy reportingPolicy) const {
  return isAllowedByAll<&CSPDirectiveList::allowManifestFromSource>(
      m_policies, url, redirectStatus, reportingPolicy);
}
