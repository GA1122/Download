void SyncTest::RestartSyncService(int index) {
  DVLOG(1) << "Restarting profile sync service for profile " << index << ".";
  delete clients_[index];
  Profile* profile = GetProfile(index);
  ProfileSyncService* service =
      ProfileSyncServiceFactory::GetForProfile(profile);
  service->ResetForTest();
  clients_[index] = new ProfileSyncServiceHarness(profile,
                                                  username_,
                                                  password_);
  service->Initialize();
  GetClient(index)->AwaitSyncRestart();
}
