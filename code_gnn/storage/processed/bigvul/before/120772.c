void BluetoothAdapterChromeOS::InputPropertyChanged(
    const dbus::ObjectPath& object_path,
    const std::string& property_name) {
  BluetoothDeviceChromeOS* device_chromeos = GetDeviceWithPath(object_path);
  if (!device_chromeos)
    return;

  BluetoothInputClient::Properties* properties =
      DBusThreadManager::Get()->GetBluetoothInputClient()->
          GetProperties(object_path);

  if (!properties ||
      property_name == properties->reconnect_mode.name())
     NotifyDeviceChanged(device_chromeos);
 }
