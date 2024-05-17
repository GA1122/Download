void BluetoothChooserDesktop::ShowDiscoveryState(DiscoveryState state) {
  bluetooth_chooser_controller_->OnDiscoveryStateChanged(state);
}
