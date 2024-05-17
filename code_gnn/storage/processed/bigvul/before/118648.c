void UserCloudPolicyManagerChromeOS::OnClientError(
    CloudPolicyClient* cloud_policy_client) {
  DCHECK_EQ(client(), cloud_policy_client);
  if (wait_for_policy_fetch_) {
    UMA_HISTOGRAM_SPARSE_SLOWLY(kUMAInitialFetchClientError,
                                cloud_policy_client->status());
  }
  CancelWaitForPolicyFetch();
}
