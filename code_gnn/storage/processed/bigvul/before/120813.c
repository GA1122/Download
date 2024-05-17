bool BluetoothDeviceChromeOS::IsPaired() const {
  BluetoothDeviceClient::Properties* properties =
      DBusThreadManager::Get()->GetBluetoothDeviceClient()->
          GetProperties(object_path_);
  DCHECK(properties);

  return properties->paired.value() || properties->trusted.value();
}
