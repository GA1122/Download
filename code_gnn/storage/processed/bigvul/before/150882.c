void BluetoothAdapter::RunPendingPowerCallbacks() {
  if (set_powered_callbacks_) {
    auto callbacks = std::move(set_powered_callbacks_);
    callbacks->powered == IsPowered() ? std::move(callbacks->callback).Run()
                                      : callbacks->error_callback.Run();
  }
}
