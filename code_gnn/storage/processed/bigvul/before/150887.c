BluetoothAdapter::StartOrStopDiscoveryCallback::StartOrStopDiscoveryCallback(
    base::OnceClosure start_callback,
    ErrorCallback start_error_callback) {
  this->start_callback = std::move(start_callback);
  this->start_error_callback = start_error_callback;
}
