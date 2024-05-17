void DeviceTokenFetcher::Initialize(DeviceManagementService* service,
                                    CloudPolicyCacheBase* cache,
                                    CloudPolicyDataStore* data_store,
                                    PolicyNotifier* notifier,
                                    DelayedWorkScheduler* scheduler) {
  service_ = service;
  cache_ = cache;
  notifier_ = notifier;
  data_store_ = data_store;
  effective_token_fetch_error_delay_ms_ = kTokenFetchErrorDelayMilliseconds;
  state_ = STATE_INACTIVE;
  scheduler_.reset(scheduler);

  if (cache_->is_unmanaged())
    SetState(STATE_UNMANAGED);
}
