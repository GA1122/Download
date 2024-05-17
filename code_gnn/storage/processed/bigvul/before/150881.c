BluetoothAdapter::RetrieveGattConnectedDevicesWithDiscoveryFilter(
    const BluetoothDiscoveryFilter& discovery_filter) {
  return std::unordered_map<BluetoothDevice*, BluetoothDevice::UUIDSet>();
}
