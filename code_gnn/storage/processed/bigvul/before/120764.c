void BluetoothAdapterChromeOS::DeviceAdded(
  const dbus::ObjectPath& object_path) {
  BluetoothDeviceClient::Properties* properties =
      DBusThreadManager::Get()->GetBluetoothDeviceClient()->
          GetProperties(object_path);
  if (properties->adapter.value() != object_path_)
    return;

  BluetoothDeviceChromeOS* device_chromeos =
      new BluetoothDeviceChromeOS(this, object_path);
  DCHECK(devices_.find(device_chromeos->GetAddress()) == devices_.end());

  devices_[device_chromeos->GetAddress()] = device_chromeos;

  FOR_EACH_OBSERVER(BluetoothAdapter::Observer, observers_,
                    DeviceAdded(this, device_chromeos));
}
