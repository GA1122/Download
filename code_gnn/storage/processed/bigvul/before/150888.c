BluetoothAdapter::StartOrStopDiscoveryCallback::StartOrStopDiscoveryCallback(
    base::Closure stop_callback,
    DiscoverySessionErrorCallback stop_error_callback) {
  this->stop_callback = stop_callback;
  this->stop_error_callback = std::move(stop_error_callback);
}
