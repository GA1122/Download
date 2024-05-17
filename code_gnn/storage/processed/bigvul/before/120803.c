uint32 BluetoothDeviceChromeOS::GetBluetoothClass() const {
  BluetoothDeviceClient::Properties* properties =
      DBusThreadManager::Get()->GetBluetoothDeviceClient()->
          GetProperties(object_path_);
  DCHECK(properties);

  return properties->bluetooth_class.value();
}
