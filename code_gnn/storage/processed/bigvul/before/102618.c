BrowserPolicyConnector::~BrowserPolicyConnector() {
#if defined(OS_CHROMEOS)
  if (device_cloud_policy_subsystem_.get())
    device_cloud_policy_subsystem_->Shutdown();
  device_cloud_policy_subsystem_.reset();
  device_data_store_.reset();
#endif

  if (user_cloud_policy_subsystem_.get())
    user_cloud_policy_subsystem_->Shutdown();
  user_cloud_policy_subsystem_.reset();
  user_policy_token_cache_.reset();
  user_data_store_.reset();
}
