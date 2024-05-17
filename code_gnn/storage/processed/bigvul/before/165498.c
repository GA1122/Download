bool ContentSecurityPolicy::AllowWasmEval(
    ScriptState* script_state,
    SecurityViolationReportingPolicy reporting_policy,
    ContentSecurityPolicy::ExceptionStatus exception_status,
    const String& script_content) const {
  bool is_allowed = true;
  for (const auto& policy : policies_) {
    is_allowed &= policy->AllowWasmEval(script_state, reporting_policy,
                                        exception_status, script_content);
  }
  return is_allowed;
}
