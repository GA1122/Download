void BluetoothAdapter::StartDiscoverySessionWithFilter(
    std::unique_ptr<BluetoothDiscoveryFilter> discovery_filter,
    const DiscoverySessionCallback& callback,
    const ErrorCallback& error_callback) {
  std::unique_ptr<BluetoothDiscoverySession> new_session(
      new BluetoothDiscoverySession(this, std::move(discovery_filter)));
  discovery_sessions_.insert(new_session.get());

  auto new_session_callbacks =
      base::WrapUnique(new StartOrStopDiscoveryCallback(
          base::BindOnce(callback, std::move(new_session)), error_callback));

  discovery_callback_queue_.push(std::move(new_session_callbacks));

  if (discovery_request_pending_) {
    return;
  }

  ProcessDiscoveryQueue();
}
