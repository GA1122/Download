ChromeMetricsServiceClient::ChromeMetricsServiceClient(
    metrics::MetricsStateManager* state_manager)
    : metrics_state_manager_(state_manager),
      waiting_for_collect_final_metrics_step_(false),
      num_async_histogram_fetches_in_progress_(0),
#if BUILDFLAG(ENABLE_PLUGINS)
      plugin_metrics_provider_(nullptr),
#endif
      weak_ptr_factory_(this) {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  RecordCommandLineMetrics();
  notification_listeners_active_ = RegisterForNotifications();
#if defined(OS_ANDROID)
  incognito_observer_ = std::make_unique<AndroidIncognitoObserver>(this);
  notification_listeners_active_ &= (incognito_observer_ != nullptr);
#endif
}
