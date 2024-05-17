UserCloudPolicyManagerFactoryChromeOS::UserCloudPolicyManagerFactoryChromeOS()
    : BrowserContextKeyedBaseFactory(
        "UserCloudPolicyManagerChromeOS",
        BrowserContextDependencyManager::GetInstance()) {
  DependsOn(SchemaRegistryServiceFactory::GetInstance());
}
