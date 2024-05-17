void UserCloudPolicyManagerChromeOS::OnInitialPolicyFetchComplete(
    bool success) {
  const base::Time now = base::Time::Now();
  UMA_HISTOGRAM_MEDIUM_TIMES(kUMAInitialFetchDelayPolicyFetch,
                             now - time_client_registered_);
  UMA_HISTOGRAM_MEDIUM_TIMES(kUMAInitialFetchDelayTotal,
                             now - time_init_started_);
  CancelWaitForPolicyFetch();
}
