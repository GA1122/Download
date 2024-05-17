void UserCloudPolicyManagerChromeOS::OnOAuth2PolicyTokenFetched(
    const std::string& policy_token,
    const GoogleServiceAuthError& error) {
  DCHECK(!client()->is_registered());
  time_token_available_ = base::Time::Now();
  if (wait_for_policy_fetch_) {
    UMA_HISTOGRAM_MEDIUM_TIMES(kUMAInitialFetchDelayOAuth2Token,
                               time_token_available_ - time_init_completed_);
  }

  if (error.state() == GoogleServiceAuthError::NONE) {
    client()->Register(em::DeviceRegisterRequest::USER, policy_token,
                       std::string(), false, std::string(), std::string());
  } else {
    CancelWaitForPolicyFetch();

    UMA_HISTOGRAM_ENUMERATION(kUMAInitialFetchOAuth2Error,
                              error.state(),
                              GoogleServiceAuthError::NUM_STATES);
    if (error.state() == GoogleServiceAuthError::CONNECTION_FAILED) {
      UMA_HISTOGRAM_SPARSE_SLOWLY(kUMAInitialFetchOAuth2NetworkError,
                                  error.network_error());
    }
  }

  token_fetcher_.reset();
}
