void UserCloudPolicyManagerChromeOS::StartRefreshSchedulerIfReady() {
  if (core()->refresh_scheduler())
    return;   

  if (wait_for_policy_fetch_)
    return;   

  if (!service() || !local_state_)
    return;   

  if (component_policy_service() &&
      !component_policy_service()->is_initialized()) {
    return;
  }

  core()->StartRefreshScheduler();
  core()->TrackRefreshDelayPref(local_state_,
                                policy_prefs::kUserPolicyRefreshRate);
}
