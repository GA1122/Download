void GaiaCookieManagerService::OnUbertokenSuccess(
    const std::string& uber_token) {
  DCHECK(requests_.front().request_type() ==
      GaiaCookieRequestType::ADD_ACCOUNT);
  VLOG(1) << "GaiaCookieManagerService::OnUbertokenSuccess"
          << " account=" << requests_.front().account_id();
  fetcher_retries_ = 0;
  uber_token_ = uber_token;

  if (!external_cc_result_fetched_ &&
      !external_cc_result_fetcher_.IsRunning()) {
    external_cc_result_fetcher_.Start();
    return;
  }

  signin_client_->DelayNetworkCall(
      base::Bind(&GaiaCookieManagerService::StartFetchingMergeSession,
                 base::Unretained(this)));
}
