  void SetPolicyValue(std::unique_ptr<base::Value> value) {
    PolicyMap policies;
    if (value) {
      policies.Set(key::kNoteTakingAppsLockScreenWhitelist,
                   POLICY_LEVEL_MANDATORY, POLICY_SCOPE_USER,
                   POLICY_SOURCE_CLOUD, std::move(value), nullptr);
    }
    UpdateProviderPolicy(policies);
  }
