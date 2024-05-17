void GaiaCookieManagerService::OnListAccountsFailure(
    const GoogleServiceAuthError& error) {
  VLOG(1) << "ListAccounts failed";
  DCHECK(requests_.front().request_type() ==
         GaiaCookieRequestType::LIST_ACCOUNTS);
  if (++fetcher_retries_ < kMaxFetcherRetries && error.IsTransientError()) {
    fetcher_backoff_.InformOfRequest(false);
    UMA_HISTOGRAM_ENUMERATION("Signin.ListAccountsRetry",
        error.state(), GoogleServiceAuthError::NUM_STATES);
    fetcher_timer_.Start(
        FROM_HERE, fetcher_backoff_.GetTimeUntilRelease(),
        base::Bind(&SigninClient::DelayNetworkCall,
                   base::Unretained(signin_client_),
                   base::Bind(
                       &GaiaCookieManagerService::StartFetchingListAccounts,
                       base::Unretained(this))));
    return;
  }

  UMA_HISTOGRAM_ENUMERATION("Signin.ListAccountsFailure",
      error.state(), GoogleServiceAuthError::NUM_STATES);
  for (auto& observer : observer_list_) {
    observer.OnGaiaAccountsInCookieUpdated(listed_accounts_,
                                           signed_out_accounts_, error);
  }
  HandleNextRequest();
}
