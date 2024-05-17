bool ContentSecurityPolicy::Subsumes(const ContentSecurityPolicy& other) const {
  if (!policies_.size() || !other.policies_.size())
    return !policies_.size();
  if (policies_.size() != 1)
    return false;

  CSPDirectiveListVector other_vector;
  for (const auto& policy : other.policies_) {
    if (!policy->IsReportOnly())
      other_vector.push_back(policy);
  }

  return policies_[0]->Subsumes(other_vector);
}
