CloudPolicySubsystem::CloudPolicySubsystem(
    CloudPolicyDataStore* data_store,
    CloudPolicyCacheBase* policy_cache) {
  std::string device_management_url;
  CommandLine* command_line = CommandLine::ForCurrentProcess();
  if (command_line->HasSwitch(switches::kDeviceManagementUrl)) {
    device_management_url =
        command_line->GetSwitchValueASCII(switches::kDeviceManagementUrl);
  }
  Initialize(data_store, policy_cache, device_management_url);
}
