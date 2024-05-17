void UserCloudPolicyManagerChromeOS::OnAccessTokenAvailable(
    const std::string& access_token) {
  access_token_ = access_token;

  if (!wildcard_username_.empty()) {
    wildcard_login_checker_.reset(new WildcardLoginChecker());
    wildcard_login_checker_->StartWithAccessToken(
        access_token,
        base::Bind(&OnWildcardCheckCompleted, wildcard_username_));
  }

  if (service() && service()->IsInitializationComplete() &&
      client() && !client()->is_registered()) {
    OnOAuth2PolicyTokenFetched(
        access_token, GoogleServiceAuthError(GoogleServiceAuthError::NONE));
  }
}
