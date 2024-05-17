void CloudPolicySubsystem::Initialize(
    CloudPolicyDataStore* data_store,
    CloudPolicyCacheBase* policy_cache,
    const std::string& device_management_url) {
  device_management_url_ = device_management_url;
  data_store_ = data_store;
  net::NetworkChangeNotifier::AddIPAddressObserver(this);
  notifier_.reset(new PolicyNotifier());
  if (!device_management_url_.empty()) {
    device_management_service_.reset(new DeviceManagementService(
        device_management_url));
    cloud_policy_cache_.reset(policy_cache);
    cloud_policy_cache_->set_policy_notifier(notifier_.get());
    cloud_policy_cache_->Load();
    CreateDeviceTokenFetcher();
  }
}
