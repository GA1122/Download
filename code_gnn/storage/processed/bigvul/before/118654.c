void UserCloudPolicyManagerChromeOS::OnRegistrationStateChanged(
    CloudPolicyClient* cloud_policy_client) {
  DCHECK_EQ(client(), cloud_policy_client);

  if (wait_for_policy_fetch_) {
    time_client_registered_ = base::Time::Now();
    if (!time_token_available_.is_null()) {
      UMA_HISTOGRAM_MEDIUM_TIMES(
          kUMAInitialFetchDelayClientRegister,
          time_client_registered_ - time_token_available_);
    }

    if (client()->is_registered()) {
      service()->RefreshPolicy(
          base::Bind(
              &UserCloudPolicyManagerChromeOS::OnInitialPolicyFetchComplete,
              base::Unretained(this)));
    } else {
      CancelWaitForPolicyFetch();
    }
  }
}
