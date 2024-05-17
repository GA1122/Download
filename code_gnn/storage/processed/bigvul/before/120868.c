void OneClickSigninSyncStarter::FinishProfileSyncServiceSetup() {
  ProfileSyncService* service =
      ProfileSyncServiceFactory::GetForProfile(profile_);
  if (service)
    service->SetSetupInProgress(false);
}
