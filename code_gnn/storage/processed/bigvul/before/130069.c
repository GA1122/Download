VariationsService::VariationsService(
    web_resource::ResourceRequestAllowedNotifier* notifier,
    PrefService* local_state,
    metrics::MetricsStateManager* state_manager)
    : local_state_(local_state),
      state_manager_(state_manager),
      policy_pref_service_(local_state),
      seed_store_(local_state),
      create_trials_from_seed_called_(false),
      initial_request_completed_(false),
      disable_deltas_for_next_request_(false),
      resource_request_allowed_notifier_(notifier),
      request_count_(0),
      weak_ptr_factory_(this) {
  resource_request_allowed_notifier_->Init(this);
}
