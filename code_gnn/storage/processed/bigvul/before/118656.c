void UserCloudPolicyManagerChromeOS::Shutdown() {
  if (client())
    client()->RemoveObserver(this);
  if (service())
    service()->RemoveObserver(this);
  token_fetcher_.reset();
  external_data_manager_->Disconnect();
  CloudPolicyManager::Shutdown();
}
