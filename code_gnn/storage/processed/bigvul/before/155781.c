DiceResponseHandler::DiceTokenFetcher::DiceTokenFetcher(
    const std::string& gaia_id,
    const std::string& email,
    const std::string& authorization_code,
    SigninClient* signin_client,
    AccountReconcilor* account_reconcilor,
    std::unique_ptr<ProcessDiceHeaderDelegate> delegate,
    DiceResponseHandler* dice_response_handler)
    : gaia_id_(gaia_id),
      email_(email),
      authorization_code_(authorization_code),
      delegate_(std::move(delegate)),
      dice_response_handler_(dice_response_handler),
      timeout_closure_(
          base::Bind(&DiceResponseHandler::DiceTokenFetcher::OnTimeout,
                     base::Unretained(this))),
      should_enable_sync_(false) {
  DCHECK(dice_response_handler_);
  account_reconcilor_lock_ =
      std::make_unique<AccountReconcilor::Lock>(account_reconcilor);
  gaia_auth_fetcher_ = signin_client->CreateGaiaAuthFetcher(
      this, GaiaConstants::kChromeSource, signin_client->GetURLLoaderFactory());
  VLOG(1) << "Start fetching token for account: " << email;
  gaia_auth_fetcher_->StartAuthCodeForOAuth2TokenExchange(authorization_code_);
  base::ThreadTaskRunnerHandle::Get()->PostDelayedTask(
      FROM_HERE, timeout_closure_.callback(),
      base::TimeDelta::FromSeconds(kDiceTokenFetchTimeoutSeconds));
}
