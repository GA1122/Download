 void CloudPolicySubsystem::StopAutoRetry() {
  cloud_policy_controller_->StopAutoRetry();
  device_token_fetcher_->StopAutoRetry();
// void CloudPolicySubsystem::Reset() {
    data_store_->Reset();
//   cloud_policy_cache_->Reset();
//   cloud_policy_controller_->Reset();
//   device_token_fetcher_->Reset();
  }