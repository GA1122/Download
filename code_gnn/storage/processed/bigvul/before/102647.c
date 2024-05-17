void CloudPolicySubsystem::CreateDeviceTokenFetcher() {
  device_token_fetcher_.reset(
      new DeviceTokenFetcher(device_management_service_.get(),
                             cloud_policy_cache_.get(),
                             data_store_,
                             notifier_.get()));
}
