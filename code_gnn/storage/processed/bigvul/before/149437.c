String ContentSecurityPolicy::evalDisabledErrorMessage() const {
  for (const auto& policy : m_policies) {
    if (!policy->allowEval(0,
                           SecurityViolationReportingPolicy::SuppressReporting))
      return policy->evalDisabledErrorMessage();
  }
  return String();
}
