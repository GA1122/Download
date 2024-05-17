BrowserPolicyConnector::BrowserPolicyConnector()
    : ALLOW_THIS_IN_INITIALIZER_LIST(method_factory_(this)) {
  managed_platform_provider_.reset(CreateManagedPlatformProvider());
  recommended_platform_provider_.reset(CreateRecommendedPlatformProvider());

  managed_cloud_provider_.reset(new CloudPolicyProviderImpl(
      ConfigurationPolicyPrefStore::GetChromePolicyDefinitionList(),
      CloudPolicyCacheBase::POLICY_LEVEL_MANDATORY));
  recommended_cloud_provider_.reset(new CloudPolicyProviderImpl(
      ConfigurationPolicyPrefStore::GetChromePolicyDefinitionList(),
      CloudPolicyCacheBase::POLICY_LEVEL_RECOMMENDED));

#if defined(OS_CHROMEOS)
  InitializeDevicePolicy();
#endif
}
