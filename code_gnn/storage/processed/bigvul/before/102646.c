void CloudPolicySubsystem::CreateCloudPolicyController() {
  DCHECK(!cloud_policy_controller_.get());
  cloud_policy_controller_.reset(
      new CloudPolicyController(device_management_service_.get(),
                                cloud_policy_cache_.get(),
                                device_token_fetcher_.get(),
                                data_store_,
                                notifier_.get()));
}
