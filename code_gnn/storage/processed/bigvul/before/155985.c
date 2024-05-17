void InlineLoginHandlerImpl::CompleteLogin(const std::string& email,
                                           const std::string& password,
                                           const std::string& gaia_id,
                                           const std::string& auth_code,
                                           bool skip_for_now,
                                           bool trusted,
                                           bool trusted_found,
                                           bool choose_what_to_sync) {
  content::WebContents* contents = web_ui()->GetWebContents();
  const GURL& current_url = contents->GetURL();

  if (skip_for_now) {
    signin::SetUserSkippedPromo(Profile::FromWebUI(web_ui()));
    SyncStarterCallback(OneClickSigninSyncStarter::SYNC_SETUP_FAILURE);
    return;
  }

  if (trusted_found)
    confirm_untrusted_signin_ = !trusted;

  DCHECK(!email.empty());
  DCHECK(!gaia_id.empty());
  DCHECK(!auth_code.empty());

  content::StoragePartition* partition =
      content::BrowserContext::GetStoragePartitionForSite(
          contents->GetBrowserContext(), signin::GetSigninPartitionURL());

  Profile* profile = Profile::FromWebUI(web_ui());
  if (IsSystemProfile(profile)) {
    ProfileManager* manager = g_browser_process->profile_manager();
    base::FilePath path = profiles::GetPathOfProfileWithEmail(manager, email);
    if (path.empty()) {
      path = UserManager::GetSigninProfilePath();
    }
    if (!path.empty()) {
      signin_metrics::Reason reason =
          signin::GetSigninReasonForPromoURL(current_url);
      if (reason == signin_metrics::Reason::REASON_REAUTHENTICATION) {
        FinishCompleteLoginParams params(
            this, partition, current_url, base::FilePath(),
            confirm_untrusted_signin_, email, gaia_id, password, auth_code,
            choose_what_to_sync, false);
        ProfileManager::CreateCallback callback =
            base::Bind(&InlineLoginHandlerImpl::FinishCompleteLogin, params);
        profiles::LoadProfileAsync(path, callback);
      } else {
        bool is_force_signin_enabled = signin_util::IsForceSigninEnabled();
        InlineLoginHandlerImpl* handler = nullptr;
        if (is_force_signin_enabled)
          handler = this;
        FinishCompleteLoginParams params(
            handler, partition, current_url, path, confirm_untrusted_signin_,
            email, gaia_id, password, auth_code, choose_what_to_sync,
            is_force_signin_enabled);
        ProfileManager::CreateCallback callback =
            base::Bind(&InlineLoginHandlerImpl::FinishCompleteLogin, params);
        if (is_force_signin_enabled) {
          profiles::LoadProfileAsync(path, callback);
        } else {
          profiles::SwitchToProfile(path, true, callback,
                                    ProfileMetrics::SWITCH_PROFILE_UNLOCK);
        }
      }
    }
  } else {
    FinishCompleteLogin(FinishCompleteLoginParams(
                            this, partition, current_url, base::FilePath(),
                            confirm_untrusted_signin_, email, gaia_id, password,
                            auth_code, choose_what_to_sync, false),
                        profile, Profile::CREATE_STATUS_CREATED);
  }
}
