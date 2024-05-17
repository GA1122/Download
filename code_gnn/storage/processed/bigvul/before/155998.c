void InlineSigninHelper::OnClientOAuthFailure(
  const GoogleServiceAuthError& error) {
  if (handler_)
    handler_->HandleLoginError(error.ToString(), base::string16());

  AboutSigninInternals* about_signin_internals =
    AboutSigninInternalsFactory::GetForProfile(profile_);
  about_signin_internals->OnRefreshTokenReceived("Failure");

  base::ThreadTaskRunnerHandle::Get()->DeleteSoon(FROM_HERE, this);
}
