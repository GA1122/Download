void CloudPolicySubsystem::Shutdown() {
  if (device_management_service_.get())
    device_management_service_->Shutdown();
  cloud_policy_controller_.reset();
  cloud_policy_cache_.reset();
  pref_change_registrar_.RemoveAll();
}
