void GaiaCookieManagerService::StartFetchingMergeSession() {
  DCHECK(!uber_token_.empty());
  gaia_auth_fetcher_.reset(signin_client_->CreateGaiaAuthFetcher(
      this, GetSourceForRequest(requests_.front()),
      signin_client_->GetURLRequestContext()));

  gaia_auth_fetcher_->StartMergeSession(uber_token_,
      external_cc_result_fetcher_.GetExternalCcResult());
}
