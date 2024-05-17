void SupervisedUserService::Init() {
  DCHECK(!did_init_);
  did_init_ = true;
  DCHECK(GetSettingsService()->IsReady());

  pref_change_registrar_.Init(profile_->GetPrefs());
  pref_change_registrar_.Add(
      prefs::kSupervisedUserId,
      base::Bind(&SupervisedUserService::OnSupervisedUserIdChanged,
          base::Unretained(this)));

  browser_sync::ProfileSyncService* sync_service =
      ProfileSyncServiceFactory::GetForProfile(profile_);
  if (sync_service)
    sync_service->AddPreferenceProvider(this);

  std::string client_id = component_updater::SupervisedUserWhitelistInstaller::
      ClientIdForProfilePath(profile_->GetPath());
  whitelist_service_.reset(new SupervisedUserWhitelistService(
      profile_->GetPrefs(),
      g_browser_process->supervised_user_whitelist_installer(), client_id));
  whitelist_service_->AddSiteListsChangedCallback(
      base::Bind(&SupervisedUserService::OnSiteListsChanged,
                 weak_ptr_factory_.GetWeakPtr()));

  SetActive(ProfileIsSupervised());
}
