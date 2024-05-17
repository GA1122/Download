void ProfileSyncService::ResetForTest() {
  Profile* profile = profile_;
  SigninManager* signin = SigninManagerFactory::GetForProfile(profile);
  ProfileSyncService::StartBehavior behavior =
      browser_defaults::kSyncAutoStarts ? ProfileSyncService::AUTO_START
                                        : ProfileSyncService::MANUAL_START;

  ProfileSyncService* old_this = this;
  this->~ProfileSyncService();
  new(old_this) ProfileSyncService(
      new ProfileSyncComponentsFactoryImpl(profile,
                                           CommandLine::ForCurrentProcess()),
      profile,
      signin,
      behavior);
}
