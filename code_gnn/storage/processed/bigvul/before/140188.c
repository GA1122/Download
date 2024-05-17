void GaiaCookieManagerService::OnListAccountsSuccess(const std::string& data) {
  VLOG(1) << "ListAccounts successful";
  DCHECK(requests_.front().request_type() ==
         GaiaCookieRequestType::LIST_ACCOUNTS);
  fetcher_backoff_.InformOfRequest(true);

  if (!gaia::ParseListAccountsData(
          data, &listed_accounts_, &signed_out_accounts_)) {
    listed_accounts_.clear();
    signed_out_accounts_.clear();
    OnListAccountsFailure(GoogleServiceAuthError(
        GoogleServiceAuthError::UNEXPECTED_SERVICE_RESPONSE));
    return;
  }

  for (gaia::ListedAccount& account : listed_accounts_) {
    DCHECK(account.id.empty());
    account.id = AccountTrackerService::PickAccountIdForAccount(
        signin_client_->GetPrefs(), account.gaia_id, account.email);
  }

  list_accounts_stale_ = false;
  HandleNextRequest();
  for (auto& observer : observer_list_) {
    observer.OnGaiaAccountsInCookieUpdated(
        listed_accounts_, signed_out_accounts_,
        GoogleServiceAuthError(GoogleServiceAuthError::NONE));
  }
}
