bool isAllowedByAll(const CSPDirectiveListVector& policies,
                    ScriptState* scriptState,
                    SecurityViolationReportingPolicy reportingPolicy,
                    ContentSecurityPolicy::ExceptionStatus exceptionStatus) {
  bool isAllowed = true;
  for (const auto& policy : policies) {
    isAllowed &=
        (policy.get()->*allowed)(scriptState, reportingPolicy, exceptionStatus);
  }
  return isAllowed;
}
