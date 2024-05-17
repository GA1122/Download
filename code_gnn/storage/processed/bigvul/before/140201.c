void GaiaCookieManagerService::StartFetchingLogOut() {
  DCHECK(requests_.front().request_type() == GaiaCookieRequestType::LOG_OUT);
  VLOG(1) << "GaiaCookieManagerService::StartFetchingLogOut";
  gaia_auth_fetcher_.reset(signin_client_->CreateGaiaAuthFetcher(
      this, GetSourceForRequest(requests_.front()),
      signin_client_->GetURLRequestContext()));
  gaia_auth_fetcher_->StartLogOut();
}
