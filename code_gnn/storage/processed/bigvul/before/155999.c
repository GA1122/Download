void InlineSigninHelper::OnClientOAuthSuccess(const ClientOAuthResult& result) {
  if (is_force_sign_in_with_usermanager_) {
    profiles::OpenBrowserWindowForProfile(
        base::Bind(&InlineSigninHelper::OnClientOAuthSuccessAndBrowserOpened,
                   base::Unretained(this), result),
        true, false, profile_, create_status_);
  } else {
    OnClientOAuthSuccessAndBrowserOpened(result, profile_, create_status_);
  }
}
