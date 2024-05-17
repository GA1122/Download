bool ContentSecurityPolicy::AllowTrustedTypePolicy(
    const String& policy_name) const {
  bool is_allowed = true;
  for (const auto& policy : policies_) {
    if (!CheckHeaderTypeMatches(CheckHeaderType::kCheckAll,
                                policy->HeaderType())) {
      continue;
    }
    is_allowed &= policy->AllowTrustedTypePolicy(policy_name);
  }

  return is_allowed;
}
