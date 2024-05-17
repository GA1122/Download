BrowserPolicyConnector* BrowserPolicyConnector::CreateForTests() {
  const ConfigurationPolicyProvider::PolicyDefinitionList*
      policy_list = ConfigurationPolicyPrefStore::
          GetChromePolicyDefinitionList();
  return new BrowserPolicyConnector(
      new policy::DummyConfigurationPolicyProvider(policy_list),
      new policy::DummyConfigurationPolicyProvider(policy_list),
      new policy::DummyCloudPolicyProvider(policy_list),
      new policy::DummyCloudPolicyProvider(policy_list));
}
