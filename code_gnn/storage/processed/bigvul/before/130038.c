scoped_ptr<VariationsService> VariationsService::Create(
    PrefService* local_state,
    metrics::MetricsStateManager* state_manager) {
  scoped_ptr<VariationsService> result;
#if !defined(GOOGLE_CHROME_BUILD)
  if (!base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kVariationsServerURL)) {
    DVLOG(1) << "Not creating VariationsService in unofficial build without --"
             << switches::kVariationsServerURL << " specified.";
    return result.Pass();
  }
#endif
  result.reset(new VariationsService(
      new web_resource::ResourceRequestAllowedNotifier(
          local_state, switches::kDisableBackgroundNetworking),
      local_state, state_manager));
  return result.Pass();
}
