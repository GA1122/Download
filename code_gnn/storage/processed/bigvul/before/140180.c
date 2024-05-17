void GaiaCookieManagerService::HandleNextRequest() {
  VLOG(1) << "GaiaCookieManagerService::HandleNextRequest";
  if (requests_.front().request_type() ==
      GaiaCookieRequestType::LIST_ACCOUNTS) {
    while (!requests_.empty() && requests_.front().request_type() ==
           GaiaCookieRequestType::LIST_ACCOUNTS) {
      requests_.pop_front();
    }
  } else {
    requests_.pop_front();
  }

  gaia_auth_fetcher_.reset();
  fetcher_retries_ = 0;
  if (requests_.empty()) {
    VLOG(1) << "GaiaCookieManagerService::HandleNextRequest: no more";
    uber_token_fetcher_.reset();
    access_token_ = std::string();
  } else {
    switch (requests_.front().request_type()) {
      case GaiaCookieRequestType::ADD_ACCOUNT:
        signin_client_->DelayNetworkCall(
            base::Bind(&GaiaCookieManagerService::StartFetchingUbertoken,
                       base::Unretained(this)));
        break;
      case GaiaCookieRequestType::LOG_OUT:
        signin_client_->DelayNetworkCall(
            base::Bind(&GaiaCookieManagerService::StartFetchingLogOut,
                       base::Unretained(this)));
        break;
      case GaiaCookieRequestType::LIST_ACCOUNTS:
        uber_token_fetcher_.reset();
        signin_client_->DelayNetworkCall(
            base::Bind(&GaiaCookieManagerService::StartFetchingListAccounts,
                       base::Unretained(this)));
        break;
    }
  }
}
