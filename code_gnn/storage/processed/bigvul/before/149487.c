bool ContentSecurityPolicy::subsumes(const ContentSecurityPolicy& other) const {
  if (!m_policies.size() || !other.m_policies.size())
    return !m_policies.size();
  if (m_policies.size() != 1)
    return false;

  CSPDirectiveListVector otherVector;
  for (const auto& policy : other.m_policies) {
    if (!policy->isReportOnly())
      otherVector.push_back(policy);
  }

  return m_policies[0]->subsumes(otherVector);
}
