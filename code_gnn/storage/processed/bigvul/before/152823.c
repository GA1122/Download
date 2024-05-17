void MetricsWebContentsObserver::FlushMetricsOnAppEnterBackground() {

  if (committed_load_)
    committed_load_->FlushMetricsOnAppEnterBackground();
  for (const auto& kv : provisional_loads_) {
    kv.second->FlushMetricsOnAppEnterBackground();
  }
  for (const auto& tracker : aborted_provisional_loads_) {
    tracker->FlushMetricsOnAppEnterBackground();
  }
}
