BrowserPolicyConnector::BrowserPolicyConnector(
    ConfigurationPolicyProvider* managed_platform_provider,
    ConfigurationPolicyProvider* recommended_platform_provider,
    CloudPolicyProvider* managed_cloud_provider,
    CloudPolicyProvider* recommended_cloud_provider)
    : managed_platform_provider_(managed_platform_provider),
      recommended_platform_provider_(recommended_platform_provider),
      managed_cloud_provider_(managed_cloud_provider),
      recommended_cloud_provider_(recommended_cloud_provider),
      ALLOW_THIS_IN_INITIALIZER_LIST(method_factory_(this)) {}
