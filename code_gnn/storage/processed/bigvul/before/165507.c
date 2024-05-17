void ContentSecurityPolicy::DidReceiveHeader(
    const String& header,
    ContentSecurityPolicyHeaderType type,
    ContentSecurityPolicyHeaderSource source) {
  AddAndReportPolicyFromHeaderValue(header, type, source);

  if (execution_context_)
    ApplyPolicySideEffectsToExecutionContext();
}
