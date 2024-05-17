void UserCloudPolicyManagerChromeOS::OnInitializationCompleted(
    CloudPolicyService* cloud_policy_service) {
  DCHECK_EQ(service(), cloud_policy_service);
  cloud_policy_service->RemoveObserver(this);

  time_init_completed_ = base::Time::Now();
  UMA_HISTOGRAM_MEDIUM_TIMES(kUMADelayInitialization,
                             time_init_completed_ - time_init_started_);

  if (!client()->is_registered()) {
    if (wait_for_policy_fetch_) {
      FetchPolicyOAuthTokenUsingSigninProfile();
    } else if (!access_token_.empty()) {
      OnAccessTokenAvailable(access_token_);
    }
  }

  if (!wait_for_policy_fetch_) {
    StartRefreshSchedulerIfReady();
  }
}
