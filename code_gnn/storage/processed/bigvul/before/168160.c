 void BluetoothChooserDesktop::SetAdapterPresence(AdapterPresence presence) {
  bluetooth_chooser_controller_->OnAdapterPresenceChanged(presence);
}
