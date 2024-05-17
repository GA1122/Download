void UserCloudPolicyManagerChromeOS::OnBlockingFetchTimeout() {
  if (!wait_for_policy_fetch_)
    return;
  LOG(WARNING) << "Timed out while waiting for the initial policy fetch. "
               << "The first session will start without policy.";
  CancelWaitForPolicyFetch();
}
