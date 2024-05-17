void BluetoothAdapter::ProcessDiscoveryQueue() {
  if (discovery_callback_queue_.empty())
    return;
  DCHECK(callbacks_awaiting_response_.empty());
  callbacks_awaiting_response_.swap(discovery_callback_queue_);

  if (NumDiscoverySessions() == 0) {
    if (internal_discovery_state_ == DiscoveryState::kIdle) {
      OnDiscoveryChangeComplete(false,
                                UMABluetoothDiscoverySessionOutcome::SUCCESS);
      return;
    }
    internal_discovery_state_ = DiscoveryState::kStopping;
    discovery_request_pending_ = true;
    StopScan(base::BindOnce(&BluetoothAdapter::OnDiscoveryChangeComplete,
                            weak_ptr_factory_.GetWeakPtr()));

    return;
  }

  auto result_callback =
      base::BindOnce(&BluetoothAdapter::OnDiscoveryChangeComplete,
                     weak_ptr_factory_.GetWeakPtr());
  auto new_desired_filter = GetMergedDiscoveryFilter();
  discovery_request_pending_ = true;
  filter_being_set_.CopyFrom(*new_desired_filter.get());
  if (internal_discovery_state_ == DiscoveryState::kDiscovering) {
    MaybeUpdateFilter(std::move(new_desired_filter),
                      std::move(result_callback));
    return;
  }
  internal_discovery_state_ = DiscoveryState::kStarting;
  StartScanWithFilter(std::move(new_desired_filter),
                      std::move(result_callback));
}