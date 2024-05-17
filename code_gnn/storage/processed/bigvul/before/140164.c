void GaiaCookieManagerService::AddAccountToCookieInternal(
    const std::string& account_id,
    const std::string& source) {
  DCHECK(!account_id.empty());
  if (!signin_client_->AreSigninCookiesAllowed()) {
    SignalComplete(account_id,
        GoogleServiceAuthError(GoogleServiceAuthError::REQUEST_CANCELED));
    return;
  }

  requests_.push_back(
      GaiaCookieRequest::CreateAddAccountRequest(account_id, source));
  if (requests_.size() == 1) {
    signin_client_->DelayNetworkCall(
        base::Bind(&GaiaCookieManagerService::StartFetchingUbertoken,
                   base::Unretained(this)));
  }
}
