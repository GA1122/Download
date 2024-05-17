String ContentSecurityPolicy::EvalDisabledErrorMessage() const {
  for (const auto& policy : policies_) {
    if (!policy->AllowEval(nullptr,
                           SecurityViolationReportingPolicy::kSuppressReporting,
                           kWillNotThrowException, g_empty_string)) {
      return policy->EvalDisabledErrorMessage();
    }
  }
  return String();
}
