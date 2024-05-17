  void SetPolicy(PolicyMap* policies,
                 const char* key,
                 std::unique_ptr<base::Value> value) {
    if (value) {
      policies->Set(key, POLICY_LEVEL_MANDATORY, POLICY_SCOPE_USER,
                    POLICY_SOURCE_CLOUD, std::move(value), nullptr);
    } else {
      policies->Erase(key);
    }
  }
