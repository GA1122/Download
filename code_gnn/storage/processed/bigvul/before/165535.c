void ContentSecurityPolicy::ReportBlockedScriptExecutionToInspector(
    const String& directive_text) const {
  probe::scriptExecutionBlockedByCSP(execution_context_, directive_text);
}
