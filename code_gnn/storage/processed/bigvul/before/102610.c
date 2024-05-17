void BrowserPolicyConnector::InitializeDevicePolicy() {
#if defined(OS_CHROMEOS)
  device_cloud_policy_subsystem_.reset();
  device_data_store_.reset();

  CommandLine* command_line = CommandLine::ForCurrentProcess();
  if (command_line->HasSwitch(switches::kEnableDevicePolicy)) {
    device_data_store_.reset(CloudPolicyDataStore::CreateForDevicePolicies());
    chromeos::CryptohomeLibrary* cryptohome = NULL;
    if (chromeos::CrosLibrary::Get()->EnsureLoaded())
      cryptohome = chromeos::CrosLibrary::Get()->GetCryptohomeLibrary();
    install_attributes_.reset(new EnterpriseInstallAttributes(cryptohome));
    DevicePolicyCache* device_policy_cache =
        new DevicePolicyCache(device_data_store_.get(),
                              install_attributes_.get());

    managed_cloud_provider_->AppendCache(device_policy_cache);
    recommended_cloud_provider_->AppendCache(device_policy_cache);

    device_cloud_policy_subsystem_.reset(new CloudPolicySubsystem(
        device_data_store_.get(),
        device_policy_cache));

    MessageLoop::current()->PostTask(
        FROM_HERE,
        method_factory_.NewRunnableMethod(
            &BrowserPolicyConnector::InitializeDevicePolicySubsystem));
  }
#endif
}
