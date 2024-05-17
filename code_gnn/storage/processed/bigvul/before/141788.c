void ChromeMetricsServiceClient::Initialize() {
  PrefService* local_state = g_browser_process->local_state();

#if defined(OS_ANDROID)
  local_state->ClearPref(prefs::kCrashReportingEnabled);
#endif

  metrics_service_.reset(
      new metrics::MetricsService(metrics_state_manager_, this, local_state));

  RegisterMetricsServiceProviders();

  if (IsMetricsReportingForceEnabled() ||
      base::FeatureList::IsEnabled(ukm::kUkmFeature)) {
    bool restrict_to_whitelist_entries = !IsMetricsReportingForceEnabled();
    ukm_service_.reset(
        new ukm::UkmService(local_state, this, restrict_to_whitelist_entries));
    ukm_service_->SetIsWebstoreExtensionCallback(
        base::BindRepeating(&IsWebstoreExtension));
    RegisterUKMProviders();
  }
}
