void OneClickSigninSyncStarter::OnPolicyFetchComplete(bool success) {
  DLOG_IF(ERROR, !success) << "Error fetching policy for user";
  DVLOG_IF(1, success) << "Policy fetch successful - completing signin";
  SigninManagerFactory::GetForProfile(profile_)->CompletePendingSignin();
}
