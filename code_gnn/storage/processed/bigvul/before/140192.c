void GaiaCookieManagerService::OnMergeSessionSuccess(const std::string& data) {
  VLOG(1) << "MergeSession successful account="
          << requests_.front().account_id();
  DCHECK(requests_.front().request_type() ==
         GaiaCookieRequestType::ADD_ACCOUNT);

  list_accounts_stale_ = true;

  const std::string account_id = requests_.front().account_id();
  HandleNextRequest();
  SignalComplete(account_id, GoogleServiceAuthError::AuthErrorNone());

  fetcher_backoff_.InformOfRequest(true);
  uber_token_ = std::string();
}
