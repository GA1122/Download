void BluetoothAdapter::NotifyDiscoveryError(CallbackQueue callback_queue) {
  while (!callback_queue.empty()) {
    std::unique_ptr<StartOrStopDiscoveryCallback> callbacks =
        std::move(callback_queue.front());
    callback_queue.pop();
    if (callbacks->start_error_callback)
      callbacks->start_error_callback.Run();
    if (callbacks->stop_callback)
      std::move(callbacks->stop_callback).Run();
  }
}
