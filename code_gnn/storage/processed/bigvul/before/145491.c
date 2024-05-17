void ResourceDispatcherHostImpl::DidStartRequest(ResourceLoader* loader) {
  if (!update_load_states_timer_->IsRunning() &&
      scheduler_->HasLoadingClients()) {
    update_load_states_timer_->Start(
        FROM_HERE, TimeDelta::FromMilliseconds(kUpdateLoadStatesIntervalMsec),
        this, &ResourceDispatcherHostImpl::UpdateLoadInfo);
  }
}
