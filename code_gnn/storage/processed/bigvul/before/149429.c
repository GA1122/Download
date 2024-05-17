void ContentSecurityPolicy::copyStateFrom(const ContentSecurityPolicy* other) {
  ASSERT(m_policies.isEmpty());
  for (const auto& policy : other->m_policies)
    addAndReportPolicyFromHeaderValue(policy->header(), policy->headerType(),
                                      policy->headerSource());
}
