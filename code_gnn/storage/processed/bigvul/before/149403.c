bool ContentSecurityPolicy::allowEval(
    ScriptState* scriptState,
    SecurityViolationReportingPolicy reportingPolicy,
    ContentSecurityPolicy::ExceptionStatus exceptionStatus) const {
  return isAllowedByAll<&CSPDirectiveList::allowEval>(
      m_policies, scriptState, reportingPolicy, exceptionStatus);
}
