void ContentSecurityPolicy::bindToExecutionContext(
    ExecutionContext* executionContext) {
  m_executionContext = executionContext;
  applyPolicySideEffectsToExecutionContext();
}
