ProfileSyncService* OneClickSigninSyncStarter::GetProfileSyncService() {
  ProfileSyncService* service = NULL;
  if (profile_->IsSyncAccessible())
    service = ProfileSyncServiceFactory::GetForProfile(profile_);
  return service;
}
