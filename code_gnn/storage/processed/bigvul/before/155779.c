DiceResponseHandler::DiceResponseHandler(
    SigninClient* signin_client,
    SigninManager* signin_manager,
    ProfileOAuth2TokenService* profile_oauth2_token_service,
    AccountTrackerService* account_tracker_service,
    AccountReconcilor* account_reconcilor,
    AboutSigninInternals* about_signin_internals,
    signin::AccountConsistencyMethod account_consistency,
    const base::FilePath& profile_path)
    : signin_manager_(signin_manager),
      signin_client_(signin_client),
      token_service_(profile_oauth2_token_service),
      account_tracker_service_(account_tracker_service),
      account_reconcilor_(account_reconcilor),
      about_signin_internals_(about_signin_internals),
      account_consistency_(account_consistency),
      profile_path_(profile_path) {
  DCHECK(signin_client_);
  DCHECK(signin_manager_);
  DCHECK(token_service_);
  DCHECK(account_tracker_service_);
  DCHECK(account_reconcilor_);
  DCHECK(about_signin_internals_);
}
