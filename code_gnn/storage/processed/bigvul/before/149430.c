void ContentSecurityPolicy::didReceiveHeader(
    const String& header,
    ContentSecurityPolicyHeaderType type,
    ContentSecurityPolicyHeaderSource source) {
  addAndReportPolicyFromHeaderValue(header, type, source);

  if (m_executionContext)
    applyPolicySideEffectsToExecutionContext();
}
