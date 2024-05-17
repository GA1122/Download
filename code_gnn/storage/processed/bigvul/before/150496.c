void SafeBrowsingPrivateEventRouter::InitRealtimeReportingClient() {
  if (!base::FeatureList::IsEnabled(kRealtimeReportingFeature))
    return;

  identity_manager_ = IdentityManagerFactory::GetForProfile(
      Profile::FromBrowserContext(context_));
  if (!identity_manager_)
    return;

  policy::DeviceManagementService* device_management_service =
      g_browser_process->browser_policy_connector()
          ->device_management_service();
  if (!device_management_service)
    return;

  std::string dm_token =
      policy::BrowserDMTokenStorage::Get()->RetrieveDMToken();
  std::string client_id =
      policy::BrowserDMTokenStorage::Get()->RetrieveClientId();

  if (dm_token.empty())
    return;

  device_management_service->ScheduleInitialization(0);

  client_ = std::make_unique<policy::CloudPolicyClient>(
       std::string(),  std::string(),
       std::string(),  std::string(),
       std::string(),  std::string(),
      device_management_service, g_browser_process->shared_url_loader_factory(),
      nullptr, policy::CloudPolicyClient::DeviceDMTokenCallback());

  if (!client_->is_registered()) {
    client_->SetupRegistration(
        dm_token, client_id,
         std::vector<std::string>());
  }
}
