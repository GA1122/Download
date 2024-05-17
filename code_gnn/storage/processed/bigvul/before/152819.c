void MetricsWebContentsObserver::BroadcastEventToObservers(
    const void* const event_key) {
  if (committed_load_)
    committed_load_->BroadcastEventToObservers(event_key);
}
