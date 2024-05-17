bool isAllowedByAll(const CSPDirectiveListVector& policies,
                    const KURL& url,
                    RedirectStatus redirectStatus,
                    SecurityViolationReportingPolicy reportingPolicy) {
  if (ContentSecurityPolicy::shouldBypassContentSecurityPolicy(url))
    return true;

  bool isAllowed = true;
  for (const auto& policy : policies) {
    isAllowed &=
        (policy.get()->*allowFromURL)(url, redirectStatus, reportingPolicy);
  }

  return isAllowed;
}
