void CloudPolicySubsystem::UpdatePolicyRefreshRate(int64 refresh_rate) {
  if (cloud_policy_controller_.get()) {
    refresh_rate = std::max(kPolicyRefreshRateMinMs, refresh_rate);
    refresh_rate = std::min(kPolicyRefreshRateMaxMs, refresh_rate);
    cloud_policy_controller_->SetRefreshRate(refresh_rate);
  }
}
