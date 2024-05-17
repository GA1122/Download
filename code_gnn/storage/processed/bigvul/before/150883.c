void BluetoothAdapter::SetPowered(bool powered,
                                  const base::Closure& callback,
                                  const ErrorCallback& error_callback) {
  if (set_powered_callbacks_) {
    ui_task_runner_->PostTask(FROM_HERE, error_callback);
    return;
  }

  if (powered == IsPowered()) {
    ui_task_runner_->PostTask(FROM_HERE, callback);
    return;
  }

  if (!SetPoweredImpl(powered)) {
    ui_task_runner_->PostTask(FROM_HERE, error_callback);
    return;
  }

  set_powered_callbacks_ = std::make_unique<SetPoweredCallbacks>();
  set_powered_callbacks_->powered = powered;
  set_powered_callbacks_->callback = callback;
  set_powered_callbacks_->error_callback = error_callback;
}
