void BluetoothAdapter::RemoveDiscoverySession(
    BluetoothDiscoverySession* discovery_session,
    const base::Closure& callback,
    DiscoverySessionErrorCallback error_callback) {
  size_t erased = discovery_sessions_.erase(discovery_session);
  DCHECK_EQ(1u, erased);

  std::unique_ptr<StartOrStopDiscoveryCallback> removal_callbacks(
      new StartOrStopDiscoveryCallback(callback, std::move(error_callback)));

  discovery_callback_queue_.push(std::move(removal_callbacks));

  if (discovery_request_pending_) {
    return;
  }

  ProcessDiscoveryQueue();
}
