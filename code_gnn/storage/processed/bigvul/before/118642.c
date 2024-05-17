void UserCloudPolicyManagerChromeOS::FetchPolicyOAuthTokenUsingSigninProfile() {
  scoped_refptr<net::URLRequestContextGetter> signin_context;
  Profile* signin_profile = chromeos::ProfileHelper::GetSigninProfile();
  if (signin_profile)
    signin_context = signin_profile->GetRequestContext();
  if (!signin_context.get()) {
    LOG(ERROR) << "No signin Profile for policy oauth token fetch!";
    OnOAuth2PolicyTokenFetched(
        std::string(), GoogleServiceAuthError(GoogleServiceAuthError::NONE));
    return;
  }

  token_fetcher_.reset(new PolicyOAuth2TokenFetcher(
      signin_context.get(),
      g_browser_process->system_request_context(),
      base::Bind(&UserCloudPolicyManagerChromeOS::OnOAuth2PolicyTokenFetched,
                 base::Unretained(this))));
  token_fetcher_->Start();
}
