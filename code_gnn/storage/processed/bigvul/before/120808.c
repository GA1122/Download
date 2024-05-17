BluetoothDeviceChromeOS::ServiceList BluetoothDeviceChromeOS::GetServices()
    const {
  BluetoothDeviceClient::Properties* properties =
      DBusThreadManager::Get()->GetBluetoothDeviceClient()->
          GetProperties(object_path_);
  DCHECK(properties);

  return properties->uuids.value();
}
