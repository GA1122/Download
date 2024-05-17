void BrowserPolicyConnector::InitializeUserPolicy(const std::string& user_name,
                                                  const FilePath& policy_dir,
                                                  TokenService* token_service) {
  user_cloud_policy_subsystem_.reset();
  user_policy_token_cache_.reset();
  user_data_store_.reset();
  registrar_.RemoveAll();

  CommandLine* command_line = CommandLine::ForCurrentProcess();
  if (command_line->HasSwitch(switches::kDeviceManagementUrl)) {
    token_service_ = token_service;
    if (token_service_) {
      registrar_.Add(this,
                     chrome::NOTIFICATION_TOKEN_AVAILABLE,
                     Source<TokenService>(token_service_));
    }

    FilePath policy_cache_dir = policy_dir.Append(kPolicyDir);
    UserPolicyCache* user_policy_cache =
        new UserPolicyCache(policy_cache_dir.Append(kPolicyCacheFile));
    user_data_store_.reset(CloudPolicyDataStore::CreateForUserPolicies());
    user_policy_token_cache_.reset(
        new UserPolicyTokenCache(user_data_store_.get(),
                                 policy_cache_dir.Append(kTokenCacheFile)));

    managed_cloud_provider_->PrependCache(user_policy_cache);
    recommended_cloud_provider_->PrependCache(user_policy_cache);
    user_cloud_policy_subsystem_.reset(new CloudPolicySubsystem(
        user_data_store_.get(),
        user_policy_cache));

    user_policy_token_cache_->Load();

    user_data_store_->set_user_name(user_name);
    if (token_service_ &&
        token_service_->HasTokenForService(
            GaiaConstants::kDeviceManagementService)) {
      user_data_store_->SetGaiaToken(token_service_->GetTokenForService(
              GaiaConstants::kDeviceManagementService));
    }

    user_cloud_policy_subsystem_->CompleteInitialization(
        prefs::kUserPolicyRefreshRate,
        kServiceInitializationStartupDelay);
  }
}
