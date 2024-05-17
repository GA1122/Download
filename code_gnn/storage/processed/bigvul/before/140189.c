void GaiaCookieManagerService::OnLogOutFailure(
    const GoogleServiceAuthError& error) {
  DCHECK(requests_.front().request_type() == GaiaCookieRequestType::LOG_OUT);
  VLOG(1) << "GaiaCookieManagerService::OnLogOutFailure";

  if (++fetcher_retries_ < kMaxFetcherRetries) {
    fetcher_backoff_.InformOfRequest(false);
    fetcher_timer_.Start(
        FROM_HERE, fetcher_backoff_.GetTimeUntilRelease(),
        base::Bind(&SigninClient::DelayNetworkCall,
                   base::Unretained(signin_client_),
                   base::Bind(&GaiaCookieManagerService::StartFetchingLogOut,
                              base::Unretained(this))));
    return;
  }

  for (auto& observer : observer_list_)
    observer.OnLogOutAccountsFromCookieCompleted(error);
  HandleNextRequest();
}
