void PeopleHandler::OnJavascriptAllowed() {
  PrefService* prefs = profile_->GetPrefs();
  profile_pref_registrar_.Init(prefs);
  profile_pref_registrar_.Add(
      prefs::kSigninAllowed,
      base::Bind(&PeopleHandler::UpdateSyncStatus, base::Unretained(this)));

  identity::IdentityManager* identity_manager(
      IdentityManagerFactory::GetInstance()->GetForProfile(profile_));
  if (identity_manager)
    identity_manager_observer_.Add(identity_manager);

  ProfileSyncService* sync_service(
      ProfileSyncServiceFactory::GetInstance()->GetForProfile(profile_));
  if (sync_service)
    sync_service_observer_.Add(sync_service);

#if BUILDFLAG(ENABLE_DICE_SUPPORT)
  AccountTrackerService* account_tracker(
      AccountTrackerServiceFactory::GetForProfile(profile_));
  if (account_tracker)
    account_tracker_observer_.Add(account_tracker);
#endif
}
