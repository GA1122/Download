bool BluetoothDeviceChromeOS::IsConnected() const {
  BluetoothDeviceClient::Properties* properties =
      DBusThreadManager::Get()->GetBluetoothDeviceClient()->
          GetProperties(object_path_);
  DCHECK(properties);

  return properties->connected.value();
}
