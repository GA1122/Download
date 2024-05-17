bool BluetoothAdapterChromeOS::IsPowered() const {
  if (!IsPresent())
    return false;

  BluetoothAdapterClient::Properties* properties =
      DBusThreadManager::Get()->GetBluetoothAdapterClient()->
          GetProperties(object_path_);

  return properties->powered.value();
}
