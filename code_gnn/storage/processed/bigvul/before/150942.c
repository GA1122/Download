BluetoothDevice* Bluetooth::GetBluetoothDeviceRepresentingDevice(
    mojom::blink::WebBluetoothDevicePtr device_ptr,
    ExecutionContext* context) {
  String& id = device_ptr->id;
  BluetoothDevice* device = device_instance_map_.at(id);
  if (!device) {
    device = MakeGarbageCollected<BluetoothDevice>(context,
                                                   std::move(device_ptr), this);
    auto result = device_instance_map_.insert(id, device);
    DCHECK(result.is_new_entry);
  }
   return device;
 }
