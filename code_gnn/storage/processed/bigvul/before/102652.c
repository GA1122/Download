void CloudPolicySubsystem::OnIPAddressChanged() {
  if (state() == CloudPolicySubsystem::NETWORK_ERROR &&
      cloud_policy_controller_.get()) {
    cloud_policy_controller_->Retry();
  }
}
