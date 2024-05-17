void UserCloudPolicyManagerChromeOS::GetChromePolicy(PolicyMap* policy_map) {
  CloudPolicyManager::GetChromePolicy(policy_map);

  if (store()->has_policy() &&
      !policy_map->Get(key::kChromeOsMultiProfileUserBehavior)) {
    policy_map->Set(key::kChromeOsMultiProfileUserBehavior,
                    POLICY_LEVEL_MANDATORY,
                    POLICY_SCOPE_USER,
                    new base::StringValue("primary-only"),
                    NULL);
  }
}
