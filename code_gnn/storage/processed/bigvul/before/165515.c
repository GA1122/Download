ContentSecurityPolicy::ExposeForNavigationalChecks() const {
  std::vector<WebContentSecurityPolicy> policies;
  for (const auto& policy : policies_) {
    policies.push_back(policy->ExposeForNavigationalChecks());
  }

  WebContentSecurityPolicyList list;
  list.policies = policies;

  if (self_source_)
    list.self_source = self_source_->ExposeForNavigationalChecks();

  return list;
}
