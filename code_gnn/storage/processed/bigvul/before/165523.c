bool ContentSecurityPolicy::HasPolicyFromSource(
    ContentSecurityPolicyHeaderSource source) const {
  for (const auto& policy : policies_) {
    if (policy->HeaderSource() == source)
      return true;
  }
  return false;
}
