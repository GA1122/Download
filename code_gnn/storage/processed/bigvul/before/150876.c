void BluetoothAdapter::RecordBluetoothDiscoverySessionStopOutcome(
    UMABluetoothDiscoverySessionOutcome outcome) {
  UMA_HISTOGRAM_ENUMERATION(
      "Bluetooth.DiscoverySession.Stop.Outcome", static_cast<int>(outcome),
      static_cast<int>(UMABluetoothDiscoverySessionOutcome::COUNT));
}
