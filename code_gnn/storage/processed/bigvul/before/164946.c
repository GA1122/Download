void ResourceDispatcherHostImpl::MaybeStartUpdateLoadInfoTimer() {
  if (!is_shutdown_ && !waiting_on_load_state_ack_ &&
      !update_load_info_timer_->IsRunning() && !pending_loaders_.empty()) {
    update_load_info_timer_->Start(
        FROM_HERE, TimeDelta::FromMilliseconds(kUpdateLoadStatesIntervalMsec),
        this, &ResourceDispatcherHostImpl::UpdateLoadInfo);
  }
}
