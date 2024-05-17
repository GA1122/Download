BluetoothRemoteGATTCharacteristic::BluetoothRemoteGATTCharacteristic(
    ExecutionContext* context,
    mojom::blink::WebBluetoothRemoteGATTCharacteristicPtr characteristic,
    BluetoothRemoteGATTService* service,
    BluetoothDevice* device)
    : ContextLifecycleObserver(context),
      m_characteristic(std::move(characteristic)),
      m_service(service),
      m_stopped(false),
      m_device(device) {
  m_properties =
      BluetoothCharacteristicProperties::create(m_characteristic->properties);
}
