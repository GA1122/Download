void CloudPolicyController::Initialize(
    DeviceManagementService* service,
    CloudPolicyCacheBase* cache,
    DeviceTokenFetcher* token_fetcher,
    CloudPolicyDataStore* data_store,
    PolicyNotifier* notifier,
    DelayedWorkScheduler* scheduler) {
  DCHECK(cache);

  service_ = service;
  cache_ = cache;
  token_fetcher_ = token_fetcher;
  data_store_ = data_store;
  notifier_ = notifier;
  state_ = STATE_TOKEN_UNAVAILABLE;
  policy_refresh_rate_ms_ = kPolicyRefreshRateInMilliseconds;
  effective_policy_refresh_error_delay_ms_ =
      kPolicyRefreshErrorDelayInMilliseconds;
  scheduler_.reset(scheduler);
  data_store_->AddObserver(this);
  if (!data_store_->device_token().empty())
    SetState(STATE_TOKEN_VALID);
  else
    SetState(STATE_TOKEN_UNAVAILABLE);
}
