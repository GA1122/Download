bool ContentSecurityPolicy::allowImageFromSource(
    const KURL& url,
    RedirectStatus redirectStatus,
    SecurityViolationReportingPolicy reportingPolicy) const {
  if (shouldBypassContentSecurityPolicy(url, SchemeRegistry::PolicyAreaImage))
    return true;
  return isAllowedByAll<&CSPDirectiveList::allowImageFromSource>(
      m_policies, url, redirectStatus, reportingPolicy);
}
