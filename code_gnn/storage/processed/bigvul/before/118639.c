void UserCloudPolicyManagerChromeOS::CancelWaitForPolicyFetch() {
  if (!wait_for_policy_fetch_)
    return;

  wait_for_policy_fetch_ = false;
  policy_fetch_timeout_.Stop();
  CheckAndPublishPolicy();
  StartRefreshSchedulerIfReady();
}
