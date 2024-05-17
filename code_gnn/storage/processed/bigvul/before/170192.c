  void ConfigurePolicyMap(PolicyMap* policies, const char* policy_name,
                          const char* whitelist_policy,
                          const char* allow_rule) {
    policies->Set(policy_name, POLICY_LEVEL_MANDATORY, POLICY_SCOPE_USER,
                  POLICY_SOURCE_CLOUD,
                  std::make_unique<base::Value>(policy_value_), nullptr);

    if (whitelist_policy) {
      std::unique_ptr<base::ListValue> list(new base::ListValue);
      if (allow_rule) {
        list->AppendString(allow_rule);
        request_url_allowed_via_whitelist_ = true;
      } else {
        list->AppendString(ContentSettingsPattern::Wildcard().ToString());
        request_url_allowed_via_whitelist_ = false;
      }
      policies->Set(whitelist_policy, POLICY_LEVEL_MANDATORY, POLICY_SCOPE_USER,
                    POLICY_SOURCE_CLOUD, std::move(list), nullptr);
    }
  }
