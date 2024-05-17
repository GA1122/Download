void UserCloudPolicyManagerChromeOS::OnComponentCloudPolicyUpdated() {
  CloudPolicyManager::OnComponentCloudPolicyUpdated();
  StartRefreshSchedulerIfReady();
}
