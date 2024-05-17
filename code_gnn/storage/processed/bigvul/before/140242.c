BluetoothRemoteGATTServer* BluetoothRemoteGATTServer::create(
    BluetoothDevice* device) {
  return new BluetoothRemoteGATTServer(device);
}
