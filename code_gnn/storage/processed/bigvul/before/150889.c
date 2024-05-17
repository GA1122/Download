void BluetoothAdapter::UpdateDiscoveryState(bool is_error) {
  if (is_error) {
    if (internal_discovery_state_ == DiscoveryState::kStarting)
      internal_discovery_state_ = DiscoveryState::kIdle;
    if (internal_discovery_state_ == DiscoveryState::kStopping)
      internal_discovery_state_ = DiscoveryState::kIdle;
    return;
  }

  if (internal_discovery_state_ == DiscoveryState::kStarting)
    internal_discovery_state_ = DiscoveryState::kDiscovering;
  if (internal_discovery_state_ == DiscoveryState::kStopping)
    internal_discovery_state_ = DiscoveryState::kIdle;
}
