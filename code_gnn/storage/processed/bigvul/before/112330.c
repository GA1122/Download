void ResourceDispatcherHostImpl::DidStartRequest(ResourceLoader* loader) {
  if (!update_load_states_timer_->IsRunning()) {
    update_load_states_timer_->Start(FROM_HERE,
        TimeDelta::FromMilliseconds(kUpdateLoadStatesIntervalMsec),
        this, &ResourceDispatcherHostImpl::UpdateLoadStates);
  }
}
