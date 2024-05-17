void OneClickSigninSyncStarter::CancelSigninAndDelete() {
  SigninManagerFactory::GetForProfile(profile_)->SignOut();
}
