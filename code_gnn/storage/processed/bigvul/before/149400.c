bool ContentSecurityPolicy::allowAncestors(
    LocalFrame* frame,
    const KURL& url,
    SecurityViolationReportingPolicy reportingPolicy) const {
  return isAllowedByAll<&CSPDirectiveList::allowAncestors>(
      m_policies, frame, url, reportingPolicy);
}
