void BluetoothAdapterChromeOS::DevicePropertyChanged(
    const dbus::ObjectPath& object_path,
    const std::string& property_name) {
  BluetoothDeviceChromeOS* device_chromeos = GetDeviceWithPath(object_path);
  if (!device_chromeos)
    return;

  BluetoothDeviceClient::Properties* properties =
      DBusThreadManager::Get()->GetBluetoothDeviceClient()->
          GetProperties(object_path);

  if (property_name == properties->bluetooth_class.name() ||
      property_name == properties->address.name() ||
      property_name == properties->alias.name() ||
      property_name == properties->paired.name() ||
      property_name == properties->trusted.name() ||
      property_name == properties->connected.name() ||
      property_name == properties->uuids.name())
    NotifyDeviceChanged(device_chromeos);

  if (property_name == properties->connected.name()) {
    int count = 0;

    for (DevicesMap::iterator iter = devices_.begin();
         iter != devices_.end(); ++iter) {
      if (iter->second->IsPaired() && iter->second->IsConnected())
        ++count;
    }

    UMA_HISTOGRAM_COUNTS_100("Bluetooth.ConnectedDeviceCount", count);
  }
}
