void GaiaCookieManagerService::StartFetchingListAccounts() {
  VLOG(1) << "GaiaCookieManagerService::ListAccounts";
  gaia_auth_fetcher_.reset(signin_client_->CreateGaiaAuthFetcher(
      this, GetSourceForRequest(requests_.front()),
      signin_client_->GetURLRequestContext()));
  gaia_auth_fetcher_->StartListAccounts();
}
