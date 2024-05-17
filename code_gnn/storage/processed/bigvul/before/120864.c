void OneClickSigninSyncStarter::ConfirmSignin(const std::string& oauth_token) {
  DCHECK(!oauth_token.empty());
  SigninManager* signin = SigninManagerFactory::GetForProfile(profile_);
  if (signin->GetAuthenticatedUsername().empty()) {
#if defined(ENABLE_CONFIGURATION_POLICY)
    policy::UserPolicySigninService* policy_service =
        policy::UserPolicySigninServiceFactory::GetForProfile(profile_);
    policy_service->RegisterPolicyClient(
        signin->GetUsernameForAuthInProgress(),
        oauth_token,
        base::Bind(&OneClickSigninSyncStarter::OnRegisteredForPolicy,
                   weak_pointer_factory_.GetWeakPtr()));
    return;
#else
    ConfirmAndSignin();
#endif
  } else {
    signin->CompletePendingSignin();
  }
}
