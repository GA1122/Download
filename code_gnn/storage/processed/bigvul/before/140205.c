void GaiaCookieManagerService::TriggerListAccounts(const std::string& source) {
  if (requests_.empty()) {
    fetcher_retries_ = 0;
    requests_.push_back(GaiaCookieRequest::CreateListAccountsRequest(source));
    signin_client_->DelayNetworkCall(
        base::Bind(&GaiaCookieManagerService::StartFetchingListAccounts,
                   base::Unretained(this)));
  } else if (std::find_if(requests_.begin(), requests_.end(),
                          [](const GaiaCookieRequest& request) {
                            return request.request_type() == LIST_ACCOUNTS;
                          }) == requests_.end()) {
    requests_.push_back(GaiaCookieRequest::CreateListAccountsRequest(source));
  }
}
