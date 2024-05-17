void ContentSecurityPolicy::reportBlockedScriptExecutionToInspector(
    const String& directiveText) const {
  probe::scriptExecutionBlockedByCSP(m_executionContext, directiveText);
}
