bool isAllowedByAll(const CSPDirectiveListVector& policies,
                    LocalFrame* frame,
                    const KURL& url,
                    SecurityViolationReportingPolicy reportingPolicy) {
  bool isAllowed = true;
  for (const auto& policy : policies)
    isAllowed &= (policy.get()->*allowed)(frame, url, reportingPolicy);
  return isAllowed;
}
