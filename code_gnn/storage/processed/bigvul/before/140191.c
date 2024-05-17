void GaiaCookieManagerService::OnMergeSessionFailure(
    const GoogleServiceAuthError& error) {
  DCHECK(requests_.front().request_type() ==
         GaiaCookieRequestType::ADD_ACCOUNT);
  VLOG(1) << "Failed MergeSession"
          << " account=" << requests_.front().account_id()
          << " error=" << error.ToString();
  if (++fetcher_retries_ < kMaxFetcherRetries && error.IsTransientError()) {
    fetcher_backoff_.InformOfRequest(false);
    UMA_HISTOGRAM_ENUMERATION("OAuth2Login.MergeSessionRetry",
        error.state(), GoogleServiceAuthError::NUM_STATES);
    fetcher_timer_.Start(
        FROM_HERE, fetcher_backoff_.GetTimeUntilRelease(),
        base::Bind(&SigninClient::DelayNetworkCall,
                   base::Unretained(signin_client_),
                   base::Bind(
                       &GaiaCookieManagerService::StartFetchingMergeSession,
                       base::Unretained(this))));
    return;
  }

  uber_token_ = std::string();
  const std::string account_id = requests_.front().account_id();

  UMA_HISTOGRAM_ENUMERATION("OAuth2Login.MergeSessionFailure",
      error.state(), GoogleServiceAuthError::NUM_STATES);
  HandleNextRequest();
  SignalComplete(account_id, error);
}
