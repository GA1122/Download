void BluetoothDeviceChooserController::OnStartDiscoverySessionFailed() {
  if (chooser_.get()) {
    chooser_->ShowDiscoveryState(
        BluetoothChooser::DiscoveryState::FAILED_TO_START);
  }
}
