void BluetoothAdapter::RecordBluetoothDiscoverySessionStartOutcome(
    UMABluetoothDiscoverySessionOutcome outcome) {
  UMA_HISTOGRAM_ENUMERATION(
      "Bluetooth.DiscoverySession.Start.Outcome", static_cast<int>(outcome),
      static_cast<int>(UMABluetoothDiscoverySessionOutcome::COUNT));
}
