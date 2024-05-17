void CloudPolicySubsystem::CompleteInitialization(
    const char* refresh_pref_name,
    int64 delay_milliseconds) {
  if (!device_management_url_.empty()) {
    DCHECK(device_management_service_.get());
    DCHECK(cloud_policy_cache_.get());
    DCHECK(device_token_fetcher_.get());

    refresh_pref_name_ = refresh_pref_name;
    CreateCloudPolicyController();
    device_management_service_->ScheduleInitialization(delay_milliseconds);

    PrefService* local_state = g_browser_process->local_state();
    DCHECK(pref_change_registrar_.IsEmpty());
    pref_change_registrar_.Init(local_state);
    pref_change_registrar_.Add(refresh_pref_name_, this);
    UpdatePolicyRefreshRate(local_state->GetInteger(refresh_pref_name_));
  }
}
