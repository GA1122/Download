bool isAllowedByAll(const CSPDirectiveListVector& policies,
                    SecurityViolationReportingPolicy reportingPolicy) {
  bool isAllowed = true;
  for (const auto& policy : policies)
    isAllowed &= (policy.get()->*allowed)(reportingPolicy);
  return isAllowed;
}
