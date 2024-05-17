BluetoothRemoteGATTCharacteristic* BluetoothRemoteGATTCharacteristic::create(
    ExecutionContext* context,
    mojom::blink::WebBluetoothRemoteGATTCharacteristicPtr characteristic,
    BluetoothRemoteGATTService* service,
    BluetoothDevice* device) {
  return new BluetoothRemoteGATTCharacteristic(
      context, std::move(characteristic), service, device);
}
