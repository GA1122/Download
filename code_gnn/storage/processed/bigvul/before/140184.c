void GaiaCookieManagerService::LogOutAllAccounts(const std::string& source) {
  VLOG(1) << "GaiaCookieManagerService::LogOutAllAccounts";

  bool log_out_queued = false;
  if (!requests_.empty()) {
    std::vector<GaiaCookieRequest> requests_to_keep;

    for (auto it = requests_.begin() + 1; it != requests_.end(); ++it) {
      if (it->request_type() == GaiaCookieRequestType::ADD_ACCOUNT) {
        GoogleServiceAuthError error(GoogleServiceAuthError::REQUEST_CANCELED);
        SignalComplete(it->account_id(), error);
      }

      if (it->request_type() != GaiaCookieRequestType::ADD_ACCOUNT)
        requests_to_keep.push_back(*it);

      if (it->request_type() == GaiaCookieRequestType::LOG_OUT)
        log_out_queued = true;
    }

    if (requests_.front().request_type() == GaiaCookieRequestType::LOG_OUT)
      log_out_queued = true;

    if (requests_.size() > 1) {
      requests_.erase(requests_.begin() + 1, requests_.end());
      requests_.insert(
          requests_.end(), requests_to_keep.begin(), requests_to_keep.end());
    }
  }

  if (!log_out_queued) {
    requests_.push_back(GaiaCookieRequest::CreateLogOutRequest(source));
    if (requests_.size() == 1) {
      fetcher_retries_ = 0;
      signin_client_->DelayNetworkCall(
          base::Bind(&GaiaCookieManagerService::StartFetchingLogOut,
                     base::Unretained(this)));
    }
  }
}
