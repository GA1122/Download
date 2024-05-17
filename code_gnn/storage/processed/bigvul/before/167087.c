void BluetoothSocketConnectFunction::ConnectToService(
    device::BluetoothDevice* device,
    const device::BluetoothUUID& uuid) {
  device->ConnectToService(
      uuid,
      base::Bind(&BluetoothSocketConnectFunction::OnConnect, this),
      base::Bind(&BluetoothSocketConnectFunction::OnConnectError, this));
}
