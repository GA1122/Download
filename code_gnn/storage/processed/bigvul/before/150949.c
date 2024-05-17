void Bluetooth::ScanEvent(mojom::blink::WebBluetoothScanResultPtr result) {
  ExecutionContext* context = ContextLifecycleObserver::GetExecutionContext();
  DCHECK(context);

  BluetoothDevice* bluetooth_device =
      GetBluetoothDeviceRepresentingDevice(std::move(result->device), context);

  HeapVector<blink::StringOrUnsignedLong> uuids;
  for (const String& uuid : result->uuids) {
    StringOrUnsignedLong value;
    value.SetString(uuid);
    uuids.push_back(value);
  }

  auto* manufacturer_data = MakeGarbageCollected<BluetoothManufacturerDataMap>(
      result->manufacturer_data);
  auto* service_data =
      MakeGarbageCollected<BluetoothServiceDataMap>(result->service_data);

  base::Optional<int8_t> rssi;
  if (result->rssi_is_set)
    rssi = result->rssi;

  base::Optional<int8_t> tx_power;
  if (result->tx_power_is_set)
    tx_power = result->tx_power;

  base::Optional<uint16_t> appearance;
  if (result->appearance_is_set)
    appearance = result->appearance;

  auto* event = MakeGarbageCollected<BluetoothAdvertisingEvent>(
      event_type_names::kAdvertisementreceived, bluetooth_device, result->name,
      uuids, appearance, tx_power, rssi, manufacturer_data, service_data);
  DispatchEvent(*event);
}
