BrowserCommandController::~BrowserCommandController() {
  ProfileSyncService* service =
      ProfileSyncServiceFactory::GetInstance()->GetForProfile(profile());
  if (service)
    service->RemoveObserver(this);

  TabRestoreService* tab_restore_service =
      TabRestoreServiceFactory::GetForProfileIfExisting(profile());
  if (tab_restore_service)
    tab_restore_service->RemoveObserver(this);
  profile_pref_registrar_.RemoveAll();
  local_pref_registrar_.RemoveAll();
  browser_->tab_strip_model()->RemoveObserver(this);
}