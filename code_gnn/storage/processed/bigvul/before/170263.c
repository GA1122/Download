void ComponentUpdaterPolicyTest::SetEnableComponentUpdates(
    bool enable_component_updates) {
  PolicyMap policies;
  policies.Set(key::kComponentUpdatesEnabled, POLICY_LEVEL_MANDATORY,
               POLICY_SCOPE_MACHINE, POLICY_SOURCE_ENTERPRISE_DEFAULT,
               base::WrapUnique(new base::Value(enable_component_updates)),
               nullptr);
  UpdateProviderPolicy(policies);
}
